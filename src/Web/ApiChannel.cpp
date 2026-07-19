#include "Web/ApiChannel.h"
#include "Web/WebServerService.h"
#include "Web/Notifier.h"
#include "IO/IOManager.h"
#include "IO/IconHelper.h"
#include "IO/TypeHelper.h"
#include <ArduinoJson.h>

void ApiChannel::registerRoutes(WebServerService& web)
{
    auto& server = web.server();
    
    server.on("/api/channels",HTTP_GET,
    [](AsyncWebServerRequest* request)
    {
        if(request->hasParam("id"))
        {
            uint16_t id=request->getParam("id")->value().toInt();
    
            const IOChannel* ch=ioManager.getChannel(id);
    
            if(!ch)
            {
                request->send(
                    404,
                    "application/json",
                    "{\"success\":false}"
                );
                return;
            }
    
            JsonDocument doc;
    
            doc["id"]=ch->id;
            doc["name"]=ch->name;
            doc["roomId"]=ch->roomId;
            doc["state"]=ch->state;
            doc["enabled"]=ch->enabled;
            doc["favorite"]=ch->favorite;
            doc["activeLow"]=ch->activeLow;
            doc["type"]=(uint8_t)ch->type;
            doc["icon"]=(uint8_t)ch->icon;
    
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
    
        JsonArray arr=doc.to<JsonArray>();
    
        for(uint16_t i=0;i<ioManager.count();i++)
        {
            const IOChannel* ch=ioManager.getAt(i);
    
            if(!ch)
                continue;
    
            JsonObject o=arr.add<JsonObject>();
    
            o["id"]=ch->id;
            o["name"]=ch->name;
            o["roomId"]=ch->roomId;
            o["state"]=ch->state;
            o["enabled"]=ch->enabled;
            o["favorite"]=ch->favorite;
            o["activeLow"]=ch->activeLow;
            o["type"]=(uint8_t)ch->type;
            o["icon"]=(uint8_t)ch->icon;
        }
    
        String out;
        serializeJson(doc,out);
    
        request->send(
            200,
            "application/json",
            out
        );
    });
    
    server.on("/api/channels",HTTP_POST,
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
                "{\"success\":false,\"message\":\"Invalid JSON\"}"
            );
            return;
        }
    
        IOChannel channel;
    
        channel.name=doc["name"]|"Channel";
        channel.roomId=doc["roomId"]|0;
        channel.enabled=doc["enabled"]|true;
        channel.favorite=doc["favorite"]|false;
        channel.activeLow=doc["activeLow"]|false;
    
        IOType type;
        if(typeFromValue(doc["type"]|0,type))
            channel.type=type;
    
        IOIcon icon;
        if(iconFromValue(doc["icon"]|0,icon))
            channel.icon=icon;
        IOChannel* created=ioManager.add(channel);
    
        if(!created)
        {
            request->send(
                500,
                "application/json",
                "{\"success\":false,\"message\":\"Cannot create channel\"}"
            );
            return;
        }
    
        ioManager.save();
    
        Notifier::channelChanged(*created);
        Notifier::roomsChanged();
    
        JsonDocument res;
    
        res["success"]=true;
        res["id"]=created->id;
        res["name"]=created->name;
    
        String out;
        serializeJson(res,out);
    
        AsyncWebServerResponse* response=
            request->beginResponse(
                201,
                "application/json",
                out
            );
    
        response->addHeader(
            "Location",
            "/api/channels?id="+String(created->id)
        );
    
        request->send(response);
    });
    
    server.on("/api/channels",HTTP_PUT,
    [](AsyncWebServerRequest*){},
    nullptr,
    [](AsyncWebServerRequest* request,
    uint8_t* data,
    size_t len,
    size_t,
    size_t)
    {
        if(!request->hasParam("id"))
        {
            request->send(
                400,
                "application/json",
                "{\"success\":false,\"message\":\"Missing id\"}"
            );
            return;
        }
    
        uint16_t id=
            request->getParam("id")->value().toInt();
    
        JsonDocument doc;
    
        if(deserializeJson(doc,data,len))
        {
            request->send(
                400,
                "application/json",
                "{\"success\":false,\"message\":\"Invalid JSON\"}"
            );
            return;
        }
    
        IOChannel* current=
            ioManager.getChannel(id);
    
        if(!current)
        {
            request->send(
                404,
                "application/json",
                "{\"success\":false,\"message\":\"Channel not found\"}"
            );
            return;
        }
    
        IOChannel updated=*current;
    
        updated.name=doc["name"]|updated.name;
        updated.roomId=doc["roomId"]|updated.roomId;
        updated.enabled=doc["enabled"]|updated.enabled;
        updated.favorite=doc["favorite"]|updated.favorite;
        updated.activeLow=doc["activeLow"]|updated.activeLow;
        updated.state=doc["state"]|updated.state;
        IOType type;
        if(typeFromValue(doc["type"]|(uint8_t)updated.type,type))
            updated.type=type;
    
        IOIcon icon;
        if(iconFromValue(doc["icon"]|(uint8_t)updated.icon,icon))
            updated.icon=icon;
    
        if(!ioManager.update(updated))
        {
            request->send(
                500,
                "application/json",
                "{\"success\":false}"
            );
            return;
        }
    
        ioManager.save();
    
        IOChannel* ch=
            ioManager.getChannel(id);
    
        if(ch)
            Notifier::channelChanged(*ch);
    
        Notifier::roomsChanged();
    
        request->send(
            200,
            "application/json",
            "{\"success\":true}"
        );
    });
    server.on("/api/channels",HTTP_DELETE,
    [](AsyncWebServerRequest* request)
    {
        if(!request->hasParam("id"))
        {
            request->send(
                400,
                "application/json",
                "{\"success\":false,\"message\":\"Missing id\"}"
            );
            return;
        }
    
        uint16_t id=
            request->getParam("id")->value().toInt();
    
        if(!ioManager.remove(id))
        {
            request->send(
                404,
                "application/json",
                "{\"success\":false,\"message\":\"Channel not found\"}"
            );
            return;
        }
    
        ioManager.save();
    
        Notifier::roomsChanged();
    
        request->send(
            200,
            "application/json",
            "{\"success\":true}"
        );
    });
    
    server.on("/api/channels/move",
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
                "{\"success\":false,\"message\":\"Invalid JSON\"}"
            );
            return;
        }
    
        uint16_t channelId=
            doc["channelId"]|0;
    
        uint16_t roomId=
            doc["roomId"]|0;
    
        if(!ioManager.assignToRoom(channelId,roomId))
        {
            request->send(
                404,
                "application/json",
                "{\"success\":false}"
            );
            return;
        }
    
        ioManager.save();
    
        IOChannel* ch=
            ioManager.getChannel(channelId);
    
        if(ch)
            Notifier::channelChanged(*ch);
    
        Notifier::roomsChanged();
    
        request->send(
            200,
            "application/json",
            "{\"success\":true}"
        );
    });
    
}
