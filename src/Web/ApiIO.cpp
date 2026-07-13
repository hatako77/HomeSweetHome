#include "Web/ApiIO.h"

#include <ArduinoJson.h>
#include "Web/WebServerService.h"
#include "IO/IOManager.h"

namespace
{
    const char* iconToString(IOIcon icon)
    {
        switch(icon)
        {
            case IOIcon::Light:       return "light";
            case IOIcon::Fan:         return "fan";
            case IOIcon::Door:        return "door";
            case IOIcon::Lock:        return "lock";
            case IOIcon::Motion:      return "motion";
            case IOIcon::Temperature: return "temperature";
            case IOIcon::Water:       return "water";
            case IOIcon::Outlet:      return "outlet";
            case IOIcon::Bell:        return "bell";
            case IOIcon::Curtain:     return "curtain";
            case IOIcon::TV:          return "tv";
            case IOIcon::Generic:     return "generic";
            default:                  return "generic";
        }
    }
    
    const char* typeToString(IOType type)
    {
        switch(type)
        {
            case IOType::DigitalInput:
                return "input";    
            case IOType::DigitalOutput:
                return "output";    
            default:
                return "unknown";
        }
    }
}

namespace ApiIO
{
    void registerRoutes(WebServerService& web)
    {
        auto& server = web.server();
    
        server.on("/api/io/toggle", HTTP_POST,
        [](AsyncWebServerRequest *request)
        {
            if (!request->hasParam("id"))
            {
                request->send(400, "text/plain", "Missing id");
                return;
            }    
            uint16_t id = request->getParam("id")->value().toInt();
    
            if (id >= ioManager.count())
            {
                request->send(404, "text/plain", "Invalid ID");
                return;
            }    
            ioManager.toggle(id);    
            request->send(200,
                          "application/json",
                          "{\"success\":true}");
        });    
        server.on("/api/io", HTTP_GET,
        [](AsyncWebServerRequest *request)
        {
            JsonDocument doc;    
            JsonArray arr = doc.to<JsonArray>();    
            for (uint16_t i = 0; i < ioManager.count(); i++)
            {
                const IOChannel* ch = ioManager.getAt(i);    
                if (!ch)
                    continue;    
                JsonObject obj = arr.add<JsonObject>();    
                obj["id"] = ch->id;
                obj["name"] = ch->name;
                obj["state"] = ch->state;
                obj["enabled"] = ch->enabled;
                obj["activeLow"] = ch->activeLow;
                obj["favorite"] = ch->favorite;
                obj["type"] = typeToString(ch->type);
                obj["icon"] = iconToString(ch->icon);
                obj["roomId"] = ch->roomId;
                obj["driverId"] = ch->address.driverId;
                obj["device"] = ch->address.device;
                obj["pin"] = ch->address.pin;
            }
    
            String out;
            serializeJson(doc, out);    
            request->send(200, "application/json", out);
        });
    }

}
