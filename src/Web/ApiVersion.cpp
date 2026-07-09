#include "Web/ApiVersion.h"

#include <ArduinoJson.h>

#include "Web/WebServerService.h"
#include "Core/Version.h"

namespace ApiVersion
{

void registerRoutes(WebServerService& web)
{
    auto& server = web.server();

    server.on("/api/version", HTTP_GET,
        [](AsyncWebServerRequest* request)
        {
            JsonDocument doc;

            doc["app"] = APP_NAME;
            doc["version"] = APP_VERSION;

            String out;
            serializeJson(doc, out);

            request->send(200, "application/json", out);
        });
}

}
