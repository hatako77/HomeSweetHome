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

    //--------------------------------------------------------
    // GET
    //--------------------------------------------------------

    server.on("/api/rooms",
    HTTP_GET,
    [](AsyncWebServerRequest* request)
    {
        if(request->hasParam("id"))
        {
            uint16_t id=request->getParam("id")->value().toInt();

            Room* room=roomManager.get(id);

            if(!room)
            {
                request->send(
                    404,
                    "application/json",
                    "{\"success\":false}"
                );
                return;
            }

            JsonDocument doc;

            doc["id"]=room->id;
            doc["name"]=room->name;

            JsonArray channels=doc["channels"].to<JsonArray>();

            for(uint16_t i=0;i<ioManager.count();i++)
            {
                const IOChannel* ch=ioManager.getAt(i);

                if(!ch)
                    continue;

                if(ch->roomId!=room->id)
                    continue;

                JsonObject c=channels.add<JsonObject>();

                c["id"]=ch->id;
                c["name"]=ch->name;
                c["state"]=ch->state;
                c["enabled"]=ch->enabled;
                c["favorite"]=ch->favorite;
                c["type"]=(uint8_t)ch->type;
                c["icon"]=(uint8_t)ch->icon;
            }

            String out;
            serializeJson(doc,out);

            request->send(
                200,
                "application/json",
                out
            );

            return;
        }

        JsonDocument doc;

        JsonArray rooms=doc.to<JsonArray>();

        for(uint16_t i=0;i<roomManager.count();i++)
        {
            Room* room=roomManager.getAt(i);

            if(!room)
                continue;

            JsonObject r=rooms.add<JsonObject>();

            r["id"]=room->id;
            r["name"]=room->name;

            JsonArray channels=r["channels"].to<JsonArray>();

            for(uint16_t j=0;j<ioManager.count();j++)
            {
                const IOChannel* ch=ioManager.getAt(j);

                if(!ch)
                    continue;

                if(ch->roomId!=room->id)
                    continue;

                JsonObject c=channels.add<JsonObject>();

                c["id"]=ch->id;
                c["name"]=ch->name;
                c["state"]=ch->state;
                c["enabled"]=ch->enabled;
                c["favorite"]=ch->favorite;
                c["type"]=(uint8_t)ch->type;
                c["icon"]=(uint8_t)ch->icon;
            }
        }

        String out;
        serializeJson(doc,out);

        request->send(
            200,
            "application/json",
            out
        );
    });

    //--------------------------------------------------------
    // POST
    //--------------------------------------------------------

    server.on("/api/rooms",
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
            request->send(
                400,
                "application/json",
                "{\"success\":false}"
            );
            return;
        }

        Room room;

        room.name=doc["name"] | "Room";

        Room* created=roomManager.add(room);

        if(!created)
        {
            request->send(
                500,
                "application/json",
                "{\"success\":false}"
            );
            return;
        }

        roomManager.save();

        JsonDocument result;

        result["success"]=true;
        result["id"]=created->id;
        result["name"]=created->name;

        String out;
        serializeJson(result,out);

        request->send(
            201,
            "application/json",
            out
        );
    });

    server.on("/api/rooms/<id>", HTTP_GET,
    [](AsyncWebServerRequest* request)
    {
        if(!request->hasParam("id"))
        {
            request->send(400,"application/json","{\"success\":false}");
            return;
        }

        uint16_t id = .toInt();

        Room* room = roomManager.get(id);

        if(!room)
        {
            request->send(404,"application/json","{\"success\":false}");
            return;
        }

        JsonDocument doc;

        doc["id"] = room->id;
        doc["name"] = room->name;

        String out;
        serializeJson(doc,out);

        request->send(200,"application/json",out);
    });

    server.on("/api/rooms",HTTP_POST,
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

        Room room;

        room.name = doc["name"] | "Room";

        Room* created = roomManager.add(room);

        if(!created)
        {
            request->send(500,"application/json","{\"success\":false}");
            return;
        }

        roomManager.save();

        JsonDocument res;

        res["success"] = true;
        res["id"] = created->id;

        String out;
        serializeJson(res,out);

        request->send(201,"application/json",out);
    });

    server.on("/api/rooms/<id>",HTTP_PUT,
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

        uint16_t id = .toInt();

        Room room;

        room.id = id;
        room.name = doc["name"] | "Room";

        if(!roomManager.update(room))
        {
            request->send(404,"application/json","{\"success\":false}");
            return;
        }

        roomManager.save();

        request->send(200,"application/json","{\"success\":true}");
    });


    server.on("/api/rooms/<id>",HTTP_DELETE,
    [](AsyncWebServerRequest*){},
    nullptr,
    [](AsyncWebServerRequest* request,
       uint8_t*,
       size_t,
       size_t,
       size_t)
    {
        if(!request->hasParam("id"))
        {
            request->send(400,"application/json","{\"success\":false}");
            return;
        }
        
        uint16_t id =
            request->getParam("id")->value().toInt();

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

        if(!roomManager.remove(id))
        {
            request->send(
                404,
                "application/json",
                "{\"success\":false}"
            );
            return;
        }

        roomManager.save();

        request->send(
            200,
            "application/json",
            "{\"success\":true}"
        );
    });

    server.on("/api/rooms/<id>/channels",HTTP_GET,
    [](AsyncWebServerRequest* request)
    {
        if(!request->hasParam("id"))
        {
            request->send(400,"application/json","{\"success\":false}");
            return;
        }
        
        uint16_t roomId = request->getParam("id")->value().toInt();

        JsonDocument doc;

        JsonArray arr = doc.to<JsonArray>();

        for(uint16_t i=0;i<ioManager.count();i++)
        {
            const IOChannel* ch = ioManager.getAt(i);

            if(!ch)
                continue;

            if(ch->roomId != roomId)
                continue;

            JsonObject o = arr.add<JsonObject>();

            o["id"] = ch->id;
            o["name"] = ch->name;
            o["state"] = ch->state;
            o["enabled"] = ch->enabled;
            o["favorite"] = ch->favorite;
            o["type"] = (uint8_t)ch->type;
            o["icon"] = (uint8_t)ch->icon;
        }

        String out;

        serializeJson(doc,out);

        request->send(
            200,
            "application/json",
            out
        );
    });
}
