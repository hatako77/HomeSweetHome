#include "Web/ApiChannel.h"
#include <ArduinoJson.h>
#include "Web/WebServerService.h"
#include "IO/IOManager.h"
#include "IO/TypeHelper.h"
#include "IO/IconHelper.h"


void ApiChannel::registerRoutes(WebServerService& web)
{
    web.server().on("/api/channels/update", HTTP_POST, [&web]()
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
    
        IOChannel* ch = ioManager.getChannel(doc["id"] | 0);
    
        if (!ch)
        {
            web.server().send(404, "application/json", "{\"success\":false}");
            return;
        }
    
        IOChannel updated = *ch;
    
        updated.name = doc["name"] | updated.name;
        updated.roomId = doc["roomId"] | updated.roomId;
        updated.enabled = doc["enabled"] | updated.enabled;
        updated.favorite = doc["favorite"] | updated.favorite;
        updated.activeLow = doc["activeLow"] | updated.activeLow;
    
        IOType type;
        if (typeFromValue(doc["type"] | (uint8_t)updated.type, type))
            updated.type = type;
    
        IOIcon icon;
        if (iconFromValue(doc["icon"] | (uint8_t)updated.icon, icon))
            updated.icon = icon;
    
        bool ok = ioManager.update(updated);
    
        web.server().send(
            ok ? 200 : 500,
            "application/json",
            ok ? "{\"success\":true}" : "{\"success\":false}"
        );
    });
    
    web.server().on("/api/channels/on", HTTP_POST, [&web]()
    {
        if (!web.server().hasArg("id"))
        {
            web.server().send(400, "application/json", "{\"success\":false}");
            return;
        }    
        bool ok = ioManager.on(web.server().arg("id").toInt());    
        web.server().send(
            ok ? 200 : 404,
            "application/json",
            ok ? "{\"success\":true}" : "{\"success\":false}"
        );
    });

    web.server().on("/api/channels/off", HTTP_POST, [&web]()
    {
        if (!web.server().hasArg("id"))
        {
            web.server().send(400, "application/json", "{\"success\":false}");
            return;
        }
    
        bool ok = ioManager.off(web.server().arg("id").toInt());
    
        web.server().send(
            ok ? 200 : 404,
            "application/json",
            ok ? "{\"success\":true}" : "{\"success\":false}"
        );
    });

    
    web.server().on("/api/channels", HTTP_GET, [&web]()
    {
        if (web.server().hasArg("id"))
        {
            uint16_t id = web.server().arg("id").toInt();
        
            const IOChannel* ch = ioManager.getChannel(id);
        
            if (!ch)
            {
                web.server().send(404, "application/json",
                    "{\"success\":false}");
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
        
            web.server().send(200, "application/json", json);
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
        web.server().send(
            200,
            "application/json",
            response
        );
    });
    web.server().on("/api/channels/toggle", HTTP_POST, [&web]()
    {
        if (!web.server().hasArg("id"))
        {
            web.server().send(400, "application/json", "{\"success\":false}");
            return;
        }
    
        uint16_t id = web.server().arg("id").toInt();
    
        bool ok = ioManager.toggle(id);
    
        if (ok)
            ioManager.save();
    
        web.server().send(
            ok ? 200 : 404,
            "application/json",
            ok ? "{\"success\":true}" : "{\"success\":false}"
        );
    });
}
