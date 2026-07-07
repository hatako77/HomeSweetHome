#include "Web/ApiRoom.h"

#include <ArduinoJson.h>
#include "Room/RoomStorage.h"
#include "Room/Room.h"
#include "Room/RoomManager.h"


void ApiRoom::registerRoutes(WebServerService& web)
{
    web.server().on("/api/rooms", HTTP_GET, [&web]()
    {
        JsonDocument doc;
    
        JsonArray arr = doc.to<JsonArray>();
    
        for (uint8_t i = 0; i < roomManager.count(); i++)
        {
            Room* room = roomManager.get(i);
    
            JsonObject o = arr.add<JsonObject>();
    
            o["id"] = room->id;
            o["name"] = room->name;
            o["icon"] = room->icon;
            o["enabled"] = room->enabled;
            o["favorite"] = room->favorite;
        }
    
        String response;
    
        serializeJson(doc, response);
    
        web.server().send(
            200,
            "application/json",
            response
        );
    });
   
    
    server.on("/api/rooms/add", HTTP_POST, [&web]()
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
        room.icon = doc["icon"] | "home";
        room.enabled = true;
        room.favorite = false;
    
        bool ok = roomManager.add(room);
    
        if (ok)
            RoomStorage::save(roomManager);
    
        web.server().send(
            ok ? 200 : 500,
            "application/json",
            ok ? "{\"success\":true}" : "{\"success\":false}"
        );
    });
}
