#include "Web/ApiOTA.h"

#include <ArduinoJson.h>

#include "Web/WebServerService.h"
#include "OTA/OTAService.h"

extern OTAService ota;

namespace ApiOTA
{

void registerRoutes(WebServerService& web)
{
    auto& server = web.server();

    server.on("/api/ota/version", HTTP_GET, [&server]()
    {
        JsonDocument doc;

        doc["current"] = ota.getCurrentVersion();
        doc["remote"] = ota.getRemoteVersion();
        doc["update"] = ota.checkForUpdate();

        String out;
        serializeJson(doc, out);

        server.send(200, "application/json", out);
    });

    server.on("/api/ota/status", HTTP_GET, [&server]()
    {
        JsonDocument doc;

        auto st = ota.getStatus();

        doc["running"] = st.running;
        doc["finished"] = st.finished;
        doc["success"] = st.success;
        doc["downloaded"] = st.downloaded;
        doc["total"] = st.total;
        doc["percent"] = st.percent;
        doc["speed"] = st.speedKB;
        doc["eta"] = st.eta;
        doc["state"] = st.state;
        doc["error"] = st.error;

        String out;
        serializeJson(doc, out);

        server.send(200, "application/json", out);
    });

        server.on("/api/ota/update", HTTP_POST, [&server]()
        {
            server.send(200, "text/plain", "OTA STARTED");
        
            xTaskCreate([](void*)
            {
                bool ok = ota.updateFirmware();
        
                Serial.println(ok ? "OTA OK" : "OTA FAILED");
        
                vTaskDelete(NULL);
            }, "ota_task", 8192, NULL, 1, NULL);
        });

    }
}
