#include "Web/ApiSettings.h"
#include "IO/TypeHelper.h"
#include "IO/IconHelper.h"
#include <ArduinoJson.h>

#include "Web/WebServerService.h"
#include "IO/IOManager.h"


namespace ApiSettings
{

    void registerRoutes(WebServerService& web)
    {
        auto& server = web.server();    
        server.on("/api/settings/io",  HTTP_POST,    
            [](AsyncWebServerRequest *request)
            {
            },    
            nullptr,    
            [](AsyncWebServerRequest *request,
               uint8_t *data,
               size_t len,
               size_t,
               size_t)
            {
                JsonDocument doc;    
                if (deserializeJson(doc, data, len))
                {
                    request->send(
                        400,
                        "text/plain",
                        "Invalid JSON"
                    );
                    return;
                }    
                JsonArray arr = doc.as<JsonArray>();    
                uint16_t i = 0;    
                for (JsonObject o : arr)
                {
                    if (i >= ioManager.count())
                        break;    
                    IOChannel* ch = ioManager.getChannel(i);    
                    if (!ch)
                    {
                        i++;
                        continue;
                    }    
                    ch->name = o["name"] | ch->name;
                    ch->enabled = o["enabled"] | ch->enabled;
                    ch->activeLow = o["activeLow"] | ch->activeLow;    
                    IOType type;
                    if (typeFromValue(o["type"] | (uint8_t)ch->type, type))
                        ch->type = type;    
                    IOIcon icon;
                    if (iconFromValue(o["icon"] | (uint8_t)ch->icon, icon))
                        ch->icon = icon;    
                    i++;
                }    
                ioManager.save();    
                request->send(200, "application/json", "{\"success\":true}");
            });
    }
}
