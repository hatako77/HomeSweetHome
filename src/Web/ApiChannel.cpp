#include "Web/ApiChannel.h"
#include <ArduinoJson.h>
#include "Web/WebServerService.h"
#include "IO/IOManager.h"
#include "IO/TypeHelper.h"
#include "IO/IconHelper.h"
#include "Web/WebSocketService.h"


void ApiChannel::registerRoutes(WebServerService& web)
{
    auto& server = web.server();
    
    server.on("/api/channels/assign", HTTP_POST,
    [](AsyncWebServerRequest *request)
    {
        if (!request->hasParam("id") || !request->hasParam("roomId"))
        {
            request->send(400, "application/json", "{\"success\":false}");
            return;
        }
    
        uint16_t channelId = request->getParam("id")->value().toInt();
        uint16_t roomId = request->getParam("roomId")->value().toInt();
    
        bool ok = ioManager.assignToRoom(channelId, roomId);
    
        request->send(
            ok ? 200 : 404,
            "application/json",
            ok ? "{\"success\":true}" : "{\"success\":false}"
        );
    });
    
    
    server.on("/api/channels/update", HTTP_POST,    
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
        IOChannel* ch = ioManager.getChannel(doc["id"] | 0);    
        if (!ch)
        {
            request->send(
                404,
                "application/json",
                "{\"success\":false}"
            );
            return;
        }    
        IOChannel updated = *ch;    
        updated.name      = doc["name"]      | updated.name;
        updated.roomId    = doc["roomId"]    | updated.roomId;
        updated.enabled   = doc["enabled"]   | updated.enabled;
        updated.favorite  = doc["favorite"]  | updated.favorite;
        updated.activeLow = doc["activeLow"] | updated.activeLow;    
        IOType type;
        if (typeFromValue(doc["type"] | (uint8_t)updated.type, type))
            updated.type = type;    
        IOIcon icon;
        if (iconFromValue(doc["icon"] | (uint8_t)updated.icon, icon))
            updated.icon = icon;    
        bool ok = ioManager.update(updated);    
        if (ok)
            ioManager.save();    
        request->send(
            ok ? 200 : 500,
            "application/json",
            ok ?
                "{\"success\":true}" :
                "{\"success\":false}"
        );
    });    
    server.on("/api/channels/on", HTTP_POST,
    [](AsyncWebServerRequest *request)
    {
        if (!request->hasParam("id"))
        {
            request->send(400, "application/json", "{\"success\":false}");
            return;
        }    
        bool ok = ioManager.on(
            request->getParam("id")->value().toInt());    
        request->send(
            ok ? 200 : 404,
            "application/json",
            ok ? "{\"success\":true}" : "{\"success\":false}"
        );
    });
   
    
    server.on("/api/channels/off", HTTP_POST,
    [](AsyncWebServerRequest *request)
    {
        if (!request->hasParam("id"))
        {
            request->send(400, "application/json", "{\"success\":false}");
            return;
        }
    
        bool ok = ioManager.off(
            request->getParam("id")->value().toInt());
    
        request->send(
            ok ? 200 : 404,
            "application/json",
            ok ? "{\"success\":true}" : "{\"success\":false}"
        );
    });
    
    server.on("/api/channels", HTTP_GET,
    [](AsyncWebServerRequest *request)
    {
        if (request->hasParam("id"))
        {
            uint16_t id =
                request->getParam("id")->value().toInt();
    
            const IOChannel* ch = ioManager.getChannel(id);
    
            if (!ch)
            {
                request->send(
                    404,
                    "application/json",
                    "{\"success\":false}"
                );
                return;
            }
    
            JsonDocument doc;
    
            doc["id"] = ch->id;
            doc["name"] = ch->name;
            doc["roomId"] = ch->roomId;
            doc["state"] = ch->state;
            doc["enabled"] = ch->enabled;
            doc["favorite"] = ch->favorite;
            doc["type"] = (uint8_t)ch->type;
            doc["icon"] = (uint8_t)ch->icon;
    
            String json;
            serializeJson(doc, json);
    
            request->send(200, "application/json", json);
            return;
        }
    
        JsonDocument doc;
    
        JsonArray arr = doc.to<JsonArray>();
    
        for (uint16_t i = 0; i < ioManager.count(); i++)
        {
            const IOChannel* ch = ioManager.getAt(i);
    
            if (!ch)
                continue;
    
            JsonObject o = arr.add<JsonObject>();
    
            o["id"] = ch->id;
            o["name"] = ch->name;
            o["roomId"] = ch->roomId;
            o["state"] = ch->state;
            o["enabled"] = ch->enabled;
            o["favorite"] = ch->favorite;
            o["type"] = (uint8_t)ch->type;
            o["icon"] = (uint8_t)ch->icon;
        }
    
        String response;
        serializeJson(doc, response);
    
        request->send(
            200,
            "application/json",
            response
        );
    });
    
    
    
server.on("/api/channels/toggle", HTTP_POST,
[](AsyncWebServerRequest *request)
{
    if (!request->hasParam("id"))
    {
        request->send(400, "application/json", "{\"success\":false}");
        return;
    }

    uint16_t id =
        request->getParam("id")->value().toInt();
    bool ok = ioManager.toggle(id);
    if(ok)
    {
        ioManager.save();    
        websocket.notifyChannel(
            id,
            ioManager.read(id)
        );
    }
    request->send(
        ok ? 200 : 404,
        "application/json",
        ok ? "{\"success\":true}" : "{\"success\":false}"
    );
});
}
