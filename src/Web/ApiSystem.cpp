#include "Web/ApiSystem.h"

#include "Core/RoomManager.h"
#include "Core/SceneManager.h"
#include "IO/IOManager.h"
#include <WiFi.h>
#include "Core/Version.h"

void ApiSystem::registerRoutes(WebServerService& server)
{
    server.on("/api/system", HTTP_GET,
    [](AsyncWebServerRequest* request)
    {
        JsonDocument doc;

        doc["firmware"] = VERSION;

        doc["wifi"] = WiFi.RSSI();

        doc["rooms"] = roomManager.count();

        doc["channels"] = ioManager.count();

        doc["scenes"] = sceneManager.count();

        doc["schedules"] = 0;

        doc["sensors"] = 0;

        String json;

        serializeJson(doc, json);

        request->send(
            200,
            "application/json",
            json);
    });
}
