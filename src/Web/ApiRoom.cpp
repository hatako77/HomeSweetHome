#include "Web/ApiRoom.h"
#include "IO/IOManager.h"
#include "IO/IOConfig.h"
#include "Core/Room.h"
#include "Core/RoomManager.h"
#include "Web/WebSocketService.h"
#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>


extern WebSocketService websocket;

void ApiRoom::registerRoutes(WebServerService& web)
{
    auto& server = web.server();
    //--------------------------------------------------
    // GET
    //--------------------------------------------------
    server.on("/api/rooms",HTTP_GET,[](AsyncWebServerRequest* request)
        {
            //--------------------------------------------------
            // GET ONE
            //--------------------------------------------------
            if(request->hasParam("id"))
            {
                uint16_t id =request->getParam("id")->value().toInt();
                Room* room = roomManager.get(id);
                if(!room)
                {
                    request->send(404,"application/json","{\"success\":false}");
                    return;
                }
                JsonDocument doc;
                doc["id"] = room->id;
                doc["name"] = room->name;
                JsonArray channels =doc["channels"].to<JsonArray>();
                for(uint16_t i=0;i<ioManager.count();i++)
                {
                    const IOChannel* ch =ioManager.getAt(i);
                    if(!ch)continue;
                    if(ch->roomId != room->id)continue;
                    JsonObject c =channels.add<JsonObject>();
                    c["id"] = ch->id;
                    c["name"] = ch->name;
                    c["state"] = ch->state;
                    c["enabled"] = ch->enabled;
                    c["favorite"] = ch->favorite;
                    c["type"] = (uint8_t)ch->type;
                    c["icon"] = (uint8_t)ch->icon;
                }
                String out;
                serializeJson(doc,out);
                request->send(200,"application/json",out);
                return;
            }
            //--------------------------------------------------
            // GET ALL
            //--------------------------------------------------
            JsonDocument doc;
            JsonArray rooms =doc.to<JsonArray>();
            for(uint16_t i=0;i<roomManager.count();i++)
            {
                Room* room =roomManager.getAt(i);
                if(!room)continue;
                JsonObject r =rooms.add<JsonObject>();
                r["id"] = room->id;
                r["name"] = room->name;
                JsonArray channels =r["channels"].to<JsonArray>();
                for(uint16_t j=0;j<ioManager.count();j++)
                {
                    const IOChannel* ch =ioManager.getAt(j);
                    if(!ch)continue;
                    if(ch->roomId != room->id)continue;
                    JsonObject c =channels.add<JsonObject>();
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
            request->send(200,"application/json",out
            );
        }
    );

    //--------------------------------------------------
    // POST
    //--------------------------------------------------

    server.on("/api/rooms",HTTP_POST,[](AsyncWebServerRequest*){},nullptr,
        [](AsyncWebServerRequest* request,uint8_t* data,size_t len,size_t,size_t)
        {
            JsonDocument doc;
            if(deserializeJson(doc,data,len))
            {
                request->send(400,"application/json","{\"success\":false,\"message\":\"Invalid JSON\"}");
                return;
            }
            Room room;
            room.name = doc["name"] | "Room";
            Room* created =roomManager.add(room);
            if(!created)
            {
                request->send(500,"application/json","{\"success\":false,\"message\":\"Cannot create room\"}");
                return;
            }
            roomManager.save();
            JsonDocument res;
            res["success"] = true;
            res["id"] = created->id;
            res["name"] = created->name;
            String out;
            serializeJson(res,out);
            AsyncWebServerResponse* response = request->beginResponse(201,"application/json",out);
            response ->addHeader("Location","/api/rooms?id=" + String(created->id));
            websocket.broadcast(R"({"type":"room","action":"changed"})");
            request->send(response);
        }
    );
    //--------------------------------------------------
    // PUT
    //--------------------------------------------------
    server.on("/api/rooms",HTTP_PUT,[](AsyncWebServerRequest*){},nullptr,
        [](AsyncWebServerRequest* request,uint8_t* data,size_t len,size_t,size_t)
        {
            if(!request->hasParam("id"))
            {
                request->send(400,"application/json","{\"success\":false,\"message\":\"Missing id\"}");
                return;
            }
            uint16_t id =request->getParam("id")->value().toInt();
            JsonDocument doc;
            if(deserializeJson(doc,data,len))
            {
                request->send(400,"application/json","{\"success\":false,\"message\":\"Invalid JSON\"}");
                return;
            }
            Room room;
            room.id = id;
            room.name = doc["name"] | "Room";
            if(!roomManager.update(room))
            {
                request->send(404,"application/json","{\"success\":false,\"message\":\"Room not found\"}");
                return;
            }
            websocket.broadcast(R"({"type":"room","action":"changed"})");
            request->send(200,"application/json","{\"success\":true}");
        }
    );
    //--------------------------------------------------
    // DELETE
    //--------------------------------------------------
    server.on("/api/rooms",HTTP_DELETE,[](AsyncWebServerRequest* request)
        {
            if(!request->hasParam("id"))
            {
                request->send(400,"application/json","{\"success\":false,\"message\":\"Missing id\"}");
                return;
            }
            uint16_t roomId =request->getParam("id")->value().toInt();
            for(uint16_t i=0;i<ioManager.count();i++)            
            {
                const IOChannel* ch =ioManager.getAt(i);
                if(!ch)continue;
                if(ch->roomId == roomId)
                {
                    request->send(409,"application/json","{\"success\":false,\"message\":\"Room contains channels\"}");
                    return;
                }
            }
            if(!roomManager.remove(roomId))
            {
                request->send(404,"application/json","{\"success\":false,\"message\":\"Room not found\"}");
                return;
            }
            websocket.broadcast(R"({"type":"room","action":"changed"})");
            request->send(200,"application/json","{\"success\":true}");
        }
    );
    //--------------------------------------------------
    // GET CHANNELS OF ROOM
    //--------------------------------------------------
    server.on("/api/rooms/channels",HTTP_GET,[](AsyncWebServerRequest* request)
        {
            if(!request->hasParam("id"))
            {
                request->send(400,"application/json","{\"success\":false,\"message\":\"Missing id\"}");
                return;
            }
            uint16_t roomId =request->getParam("id")->value().toInt();
            JsonDocument doc;
            JsonArray arr =doc.to<JsonArray>();
            for(uint16_t i=0;i<ioManager.count();i++)
            {
                const IOChannel* ch =ioManager.getAt(i);
                if(!ch)continue;
                if(ch->roomId != roomId)continue;
                JsonObject c =arr.add<JsonObject>();
                c["id"] = ch->id;
                c["name"] = ch->name;
                c["state"] = ch->state;
                c["enabled"] = ch->enabled;
                c["favorite"] = ch->favorite;
                c["type"] = (uint8_t)ch->type;
                c["icon"] = (uint8_t)ch->icon;
            }
            String out;
            serializeJson(doc,out);
            request->send(200,"application/json",out);
        }
    );
}
