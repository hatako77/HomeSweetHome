#include "Web/ApiChannel.h"
#include <ArduinoJson.h>
#include "Web/WebServerService.h"
#include "IO/IOManager.h"



void ApiChannel::registerRoutes(WebServerService& web)
{
    web.server().on("/api/channels", HTTP_GET, [&web]()
    {
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
}
