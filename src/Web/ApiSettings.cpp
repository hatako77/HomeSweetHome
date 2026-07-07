#include "Web/ApiSettings.h"
#include "IO/TypeHelper.h"
#include "IO/IconHelper.h"
#include <ArduinoJson.h>

#include "Web/WebServerService.h"
#include "IO/IOManager.h"
#include "IO/IOStorage.h"

namespace ApiSettings
{

void registerRoutes(WebServerService& web)
{
    auto& server = web.server();

    server.on("/api/settings/io", HTTP_POST, [&server]()
    {
        if (!server.hasArg("plain"))
        {
            server.send(400, "text/plain", "Missing body");
            return;
        }

        JsonDocument doc;

        if (deserializeJson(doc, server.arg("plain")))
        {
            server.send(400, "text/plain", "Invalid JSON");
            return;
        }

        JsonArray arr = doc.as<JsonArray>();

        uint8_t i = 0;

        for (JsonObject o : arr)
        {
            if (i >= ioManager.count())
                break;

            IOChannel* ch = ioManager.getChannel(i);

            ch->name = o["name"] | ch->name;
            ch->enabled = o["enabled"] | true;
            ch->activeLow = o["activeLow"] | false;
            IOType type;
            if (typeFromValue(o["type"] | 1, type))
                ch->type = type;
            i++;
        }

        IOStorage::save(ioManager);
        server.send(200, "application/json", "{\"success\":true}");
    });
}

}
