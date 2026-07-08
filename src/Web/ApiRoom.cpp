#include "Web/ApiRoom.h"

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
