#include "Web/ApiRoom.h"

#include <ArduinoJson.h>

#include "Room/RoomManager.h"


void ApiRoom::registerRoutes(WebServer& server)
{

    server.on("/api/rooms", HTTP_GET, []()
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


        server.send(
            200,
            "application/json",
            response
        );

    });

}
