#include "Web/ApiRoom.h"
#include "IO/IOManager.h"
#include "IO/IOConfig.h"
#include <ArduinoJson.h>
#include "Core/Room.h"
#include "Repositories/RoomRepository.h"
#include <ESPAsyncWebServer.h>

void ApiRoom::registerRoutes(WebServerService& web)
{
    auto& server = web.server();
    server.on("/api/rooms", HTTP_GET,
    [](AsyncWebServerRequest *request)
    {
        if(request->hasParam("id"))
        {
            uint16_t id =
                request->getParam("id")->value().toInt();
    
            Room* room = roomRepository.get(id);
    
            if(!room)
            {
                request->send(404,
                              "application/json",
                              "{\"success\":false}");
                return;
            }
    
            JsonDocument doc;
    
            doc["id"]=room->id;
            doc["name"]=room->name;
    
            String out;
            serializeJson(doc,out);
    
            request->send(200,"application/json",out);
            return;
        }
    
        JsonDocument doc;
    
        JsonArray arr = doc.to<JsonArray>();
    
        for(uint16_t i=0;i<roomRepository.count();i++)
        {
            Room* room = roomRepository.get(i);
    
            if(!room)
                continue;
    
            JsonObject o = arr.add<JsonObject>();
    
            o["id"]=room->id;
            o["name"]=room->name;
        }
    
        String out;
    
        serializeJson(doc,out);
    
        request->send(200,"application/json",out);
    });
    
    
    
    server.on("/api/rooms",HTTP_POST,    
    [](AsyncWebServerRequest *request){},nullptr,    
    [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t, size_t)
    {
        JsonDocument doc;    
        if(deserializeJson(doc,data,len))
        {
            request->send(400,
                          "application/json",
                          "{\"success\":false}");
            return;
        }    
        Room room;    
        room.name=doc["name"]|"Room";    
        Room* created=roomRepository.add(room);    
        if(created)
            roomRepository.save();    
        request->send(created?201:500,
                      "application/json",
                      created?
                      "{\"success\":true}":
                      "{\"success\":false}");
    });
    
    server.on("/api/rooms/update", HTTP_POST,    
    [](AsyncWebServerRequest *request){}, nullptr,    
    [](AsyncWebServerRequest *request, uint8_t *data,
    size_t len, size_t, size_t)
    {
        JsonDocument doc;    
        if (deserializeJson(doc, data, len))
        {
            request->send(
                400,
                "application/json",
                "{\"success\":false,\"message\":\"Invalid JSON\"}"
            );
            return;
        }    
        Room room;    
        room.id   = doc["id"] | 0;
        room.name = doc["name"] | "Room";    
        bool ok = roomRepository.update(room);    
        if (ok)
            roomRepository.save();    
        request->send(
            ok ? 200 : 404,
            "application/json",
            ok ?
                "{\"success\":true}" :
                "{\"success\":false,\"message\":\"Room not found\"}"
        );
    });
    
    server.on("/api/rooms/channels",HTTP_GET,
    [](AsyncWebServerRequest *request)
    {
        if(!request->hasParam("id"))
        {
            request->send(400,"text/plain","Missing room id");
            return;
        }
    
        uint16_t roomId =
            request->getParam("id")->value().toInt();
    
        JsonDocument doc;
    
        JsonArray arr=doc.to<JsonArray>();
    
        for(uint16_t i=0;i<ioManager.count();i++)
        {
            const IOChannel* ch=ioManager.getAt(i);
    
            if(!ch)
                continue;
    
            if(ch->roomId!=roomId)
                continue;
    
            JsonObject o=arr.add<JsonObject>();
    
            o["id"]=ch->id;
            o["name"]=ch->name;
            o["state"]=ch->state;
        }
    
        String out;
    
        serializeJson(doc,out);
    
        request->send(200,"application/json",out);
    });
    
    server.on("/api/rooms/add", HTTP_POST,    
    [](AsyncWebServerRequest *request){}, nullptr,    
    [](AsyncWebServerRequest *request,
    uint8_t *data, size_t len, size_t, size_t)
    {
        JsonDocument doc;    
        if (deserializeJson(doc, data, len))
        {
            request->send(
                400,
                "application/json",
                "{\"success\":false}"
            );
            return;
        }    
        Room room;    
        room.name = doc["name"] | "Room";    
        Room* created = roomRepository.add(room);    
        if (created)
            roomRepository.save();    
        request->send(
            created ? 200 : 500,
            "application/json",
            created ?
                "{\"success\":true}" :
                "{\"success\":false}"
        );
    });
}
