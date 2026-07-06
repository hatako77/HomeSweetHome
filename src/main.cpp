#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <LittleFS.h>

#include "IO/IOStorage.h"
#include "Core/Version.h"
#include "OTA/OTAService.h"
#include "IO/IOManager.h"
#include "Web/ApiIO.h"


// Web UI
#include "WebUI/index_html.h"
#include "WebUI/style_css.h"
#include "WebUI/app_js.h"
#include "WebUI/ota_html.h"

// WiFi
const char* ssid = "Ali TT";
const char* password = "2150068486";

// Web Server
WebServer server(80);
OTAService ota;
TaskHandle_t otaTaskHandle = nullptr;

void otaTask(void *parameter)
{
    bool result = ota.updateFirmware();

    if (result)
    {
        ota.getStatus().state = "Rebooting";
        delay(2000);
        ESP.restart();
    }

    otaTaskHandle = nullptr;
    vTaskDelete(NULL);
}

void handleRoot()
{
    server.send_P(200, "text/html", INDEX_HTML);
}

void handleCSS()
{
    server.send_P(200, "text/css", STYLE_CSS);
}

void handleJS()
{
    server.send_P(200, "application/javascript", APP_JS);
}

void handleUpdate()
{
    if (otaTaskHandle != nullptr)
    {
        server.send(409, "text/plain", "OTA already running");
        return;
    }

    if (!ota.checkForUpdate())
    {
        server.send(404, "text/plain", "No update");
        return;
    }

    server.send(200, "text/plain", "OTA Started");

    xTaskCreatePinnedToCore(
        otaTask,
        "OTA",
        12000,
        NULL,
        1,
        &otaTaskHandle,
        1
    );
}

void setup()
{
    Serial.begin(115200);

    WiFi.begin(ssid, password);

    Serial.print("Connecting WiFi");

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected");
    Serial.println(WiFi.localIP());

    ota.setCurrentVersion(APP_VERSION);
    ota.setVersionURL("https://github.com/hatako77/HomeSweetHome/releases/latest/download/version.json");

    ioManager.begin();

    LittleFS.begin(true);
    IOStorage::load();

    // Pages
    server.on("/", handleRoot);
    server.on("/style.css", handleCSS);
    server.on("/app.js", handleJS);

    server.on("/ota", HTTP_GET, []()
    {
        server.send_P(200, "text/html", OTA_HTML);
    });

    // OTA API
    server.on("/api/ota/version", HTTP_GET, []()
    {
        JsonDocument doc;

        doc["current"] = ota.getCurrentVersion();
        doc["remote"] = ota.getRemoteVersion();
        doc["update"] = ota.checkForUpdate();

        String out;
        serializeJson(doc, out);

        server.send(200, "application/json", out);
    });

    server.on("/api/ota/status", HTTP_GET, []()
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

    server.on("/api/ota/update", HTTP_POST, []()
    {
        handleUpdate();
    });

    server.begin();

    Serial.println("HTTP server started");
}

void loop()
{
    ioManager.update();
    server.handleClient();
}
