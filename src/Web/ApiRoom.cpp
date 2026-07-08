#include "Web/ApiRoom.h"
#include "IO/IOManager.h"
#include "IO/IOConfig.h"
#include <ArduinoJson.h>
#include "Core/Room.h"
#include "Repositories/RoomRepository.h"


void ApiRoom::registerRoutes(WebServerService& web)
{
    web.server().on("/api/rooms", HTTP_GET, [&web]()
    {
        JsonDocument doc;
    
        JsonArray arr = doc.to<JsonArray>();
    
        for (uint8_t i = 0; i < roomRepository.count(); i++)
        {
            Room* room = roomRepository.get(i);
    
            JsonObject o = arr.add<JsonObject>();
    
            o["id"] = room->id;
            o["name"] = room->name;
        }
    
        String response;
    
        serializeJson(doc, response);
    
        web.server().send(
            200,
            "application/json",
            response
        );
    });
   
    web.server().on("/api/rooms/update", HTTP_POST, [&web]()
    {
        if (!web.server().hasArg("plain"))
        {
            web.server().send(400, "application/json",
                "{\"success\":false,\"message\":\"Missing body\"}");
            return;
        }
    
        JsonDocument doc;
    
        if (deserializeJson(doc, web.server().arg("plain")))
        {
            web.server().send(400, "application/json",
                "{\"success\":false,\"message\":\"Invalid JSON\"}");
            return;
        }
    
        Room room;
    
        room.id = doc["id"] | 0;
        room.name = doc["name"] | "Room";
    
        bool ok = roomRepository.update(room);
    
        if (ok)
            roomRepository.save();
    
        web.server().send(
            ok ? 200 : 404,
            "application/json",
            ok
                ? "{\"success\":true}"
                : "{\"success\":false,\"message\":\"Room not found\"}"
        );
    });
    
    web.server().on("/api/rooms/channels", HTTP_GET, [&web]()
    {
        if (!web.server().hasArg("id"))
        {
            web.server().send(400, "text/plain", "Missing room id");
            return;
        }
    
        uint8_t roomId = web.server().arg("id").toInt();
    
        JsonDocument doc;
        JsonArray arr = doc.to<JsonArray>();
    
        for (uint16_t i = 0; i < ioManager.count(); i++)
        {
            IOChannel* ch = ioManager.getChannel(i);
    
            if (!ch)
                continue;
    
            if (ch->roomId != roomId)
                continue;
    
            JsonObject o = arr.add<JsonObject>();
    
            o["id"] = ch->id;
            o["name"] = ch->name;
            o["state"] = ch->state;
            o["type"] = static_cast<uint8_t>(ch->type);
        }
    
        String response;
        serializeJson(doc, response);
    
        web.server().send(
            200,
            "application/json",
            response
        );
    });
    
    web.server().on("/api/rooms/add", HTTP_POST, [&web]()
    {
        if (!web.server().hasArg("plain"))
        {
            web.server().send(400, "application/json", "{\"success\":false}");
            return;
        }
    
        JsonDocument doc;
    
        if (deserializeJson(doc, web.server().arg("plain")))
        {
            web.server().send(400, "application/json", "{\"success\":false}");
            return;
        }
    
        Room room;
    
        room.name = doc["name"] | "Room";
    
        bool ok = roomRepository.add(room);
    
        if (ok)
            roomRepository.save();
    
        web.server().send(
            ok ? 200 : 500,
            "application/json",
            ok ? "{\"success\":true}" : "{\"success\":false}"
        );
    });
}
