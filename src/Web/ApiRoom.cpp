#include "Web/ApiRoom.h"
#include "IO/IOManager.h"
#include "IO/IOConfig.h"
#include <ArduinoJson.h>
#include "Core/Room.h"
#include "Core/RoomManager.h"
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
    
            Room* room = roomManager.getAt(id);
    
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
    
        for(uint16_t i=0;i<roomManager.count();i++)
        {
            Room* room = roomManager.getAt(i);
        
            if(!room)
                continue;
        
            JsonObject o = arr.add<JsonObject>();
        
            o["id"] = room->id;
            o["name"] = room->name;
        
            JsonArray channels = o["channels"].to<JsonArray>();
        
            for(uint16_t j=0;j<ioManager.count();j++)
            {
                const IOChannel* ch = ioManager.getAt(j);
        
                if(!ch)
                    continue;
        
                if(ch->roomId != room->id)
                    continue;
        
                JsonObject c = channels.add<JsonObject>();
        
                c["id"] = ch->id;
                c["name"] = ch->name;
                c["state"] = ch->state;
                c["enabled"] = ch->enabled;
                c["favorite"] = ch->favorite;
                c["type"] = (uint8_t)ch->type;
                c["icon"] = (uint8_t)ch->icon;
            }
        }    
        String out;
    
        serializeJson(doc,out);
    
        request->send(200,"application/json",out);
    });
    
    
    
    server.on("/api/rooms/delete",
    HTTP_POST,
    [](AsyncWebServerRequest*){},
    nullptr,
    [](AsyncWebServerRequest* request,
       uint8_t* data,
       size_t len,
       size_t,
       size_t)
    {
        JsonDocument doc;
    
        if(deserializeJson(doc,data,len))
        {
            request->send(400,"application/json","{\"success\":false}");
            return;
        }
    
        uint16_t id = doc["id"] | 0;
    
        bool hasChannels = false;
    
        for(uint16_t i=0;i<ioManager.count();i++)
        {
            const IOChannel* ch = ioManager.getAt(i);
    
            if(!ch)
                continue;
    
            if(ch->roomId == id)
            {
                hasChannels = true;
                break;
            }
        }
    
        if(hasChannels)
        {
            request->send(
                409,
                "application/json",
                "{\"success\":false,\"message\":\"Room contains channels\"}"
            );
            return;
        }
    
        bool ok = roomManager.remove(id);
    
        request->send(
            ok ? 200 : 404,
            "application/json",
            ok ? "{\"success\":true}" : "{\"success\":false}"
        );
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
        bool ok = roomManager.update(room);    
        if (ok)
            roomManager.save();    
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
        Serial.println("POST /api/rooms/add");
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
        Serial.println(room.name);
        Room* created = roomManager.add(room);    
        if (created)
            roomManager.save();    
        request->send(
            created ? 200 : 500,
            "application/json",
            created ?
                "{\"success\":true}" :
                "{\"success\":false}"
        );
    });
}
