#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include "IO/IOStorage.h"
#include "Core/Version.h"
#include "OTA/OTAService.h"
#include "IO/IOManager.h"

// Web UI (PROGMEM)
#include "WebUI/index_html.h"
#include "WebUI/style_css.h"
#include "WebUI/app_js.h"
#include "WebUI/ota_html.h"

// ===== WiFi Config =====
const char* ssid = "Ali TT";
const char* password = "2150068486";

// ===== Web Server =====
WebServer server(80);
OTAService ota;
TaskHandle_t otaTaskHandle = nullptr;

// =========================
// Handlers
// =========================
void otaTask(void *parameter)
{
    bool result = ota.updateFirmware();

    if(result)
    {
              ota.getStatus().state = "Rebooting";

        delay(2000);
        ESP.restart();
    }

    otaTaskHandle = nullptr;

    vTaskDelete(NULL);
}

void handleRoot() {
  server.send_P(200, "text/html", INDEX_HTML);
}


void handleUpdate()
{
    if(otaTaskHandle != nullptr)
    {
        server.send(409, "text/plain", "OTA already running");
        return;
    }

    if(!ota.checkForUpdate())
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

void handleCSS() {
  server.send_P(200, "text/css", STYLE_CSS);
}

void handleJS() {
  server.send_P(200, "application/javascript", APP_JS);
}



// =========================
// Setup
// =========================
void setup() {
  Serial.begin(115200);

  // WiFi connect
  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());
  ota.setCurrentVersion(APP_VERSION);
  ota.setVersionURL("https://github.com/hatako77/HomeSweetHome/releases/latest/download/version.json");

  
  // Routes
  server.on("/", handleRoot);
  server.on("/style.css", handleCSS);
  server.on("/app.js", handleJS);
  server.on("/ota", []() {
    server.send_P(200, "text/html", OTA_HTML);
  });
  ioManager.begin();
  LittleFS.begin(true);
  IOStorage::load();
  server.begin();
  Serial.println("HTTP server started");
}

// =========================
// Loop
// =========================
void loop() {
  ioManager.update();
  server.handleClient();
}
