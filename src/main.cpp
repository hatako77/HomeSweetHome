#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "Core/Version.h"
#include "Services/OTAService.h"
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
IOManager ioManager;
TaskHandle_t otaTaskHandle = nullptr;
// ===== Relay pins =====
const int relayPins[] = {16, 17};
const int relayCount = 2;

bool relayState[2] = {false, false};

// =========================
// Relay Control
// =========================
void setRelay(int ch, bool state) {
  if (ch < 0 || ch >= relayCount) return;

  relayState[ch] = state;
  digitalWrite(relayPins[ch], state ? HIGH : LOW);
}

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
void handleOtaVersion() {

  Serial.println("OTA Version Request");

  bool ok = ota.checkForUpdate();

  Serial.print("checkForUpdate: ");
  Serial.println(ok);

  StaticJsonDocument<256> doc;
  doc["success"] = ok;
  doc["version"] = ota.getRemoteVersion();

  String out;
  serializeJson(doc, out);

  server.send(200, "application/json", out);
}

void handleRoot() {
  server.send_P(200, "text/html", INDEX_HTML);
}
void handleOTAStatus() {

  OTAStatus &s = ota.getStatus();

  StaticJsonDocument<512> doc;

  doc["running"] = s.running;
  doc["finished"] = s.finished;
  doc["success"] = s.success;

  doc["downloaded"] = s.downloaded;
  doc["total"] = s.total;

  doc["percent"] = s.percent;

  doc["speed"] = s.speedKB;

  doc["eta"] = s.eta;

  doc["state"] = s.state;
  doc["error"] = s.error;

  String out;
  serializeJson(doc, out);

  server.send(200, "application/json", out);
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

void handleVersion() {
  StaticJsonDocument<256> doc;
  doc["app"] = APP_NAME;
  doc["version"] = APP_VERSION;
  doc["build"] = APP_BUILD;

  String out;
  serializeJson(doc, out);

  server.send(200, "application/json", out);
}

void handleRelay() {
  if (!server.hasArg("ch")) {
    server.send(400, "text/plain", "Missing channel");
    return;
  }

  int ch = server.arg("ch").toInt() - 1;

  bool newState = !relayState[ch];
  setRelay(ch, newState);

  server.send(200, "text/plain", newState ? "ON" : "OFF");
}

// =========================
// Setup
// =========================
void setup() {
  Serial.begin(115200);

  // Relay init
  for (int i = 0; i < relayCount; i++) {
    pinMode(relayPins[i], OUTPUT);
    setRelay(i, false);
  }

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

  //if (ota.checkForUpdate()) {
  //  Serial.println("Update available! Installing...");

  //  if (ota.updateFirmware()) {
  //    Serial.println("Update OK. Restarting...");
  //    delay(2000);
  //    ESP.restart();
  //  } else {
  //    Serial.println("Update failed!");
  //  }
  //}
  // Routes
  server.on("/", handleRoot);
  server.on("/style.css", handleCSS);
  server.on("/app.js", handleJS);
  server.on("/version", handleVersion);
  server.on("/relay", handleRelay);
  server.on("/update", handleUpdate);
  server.on("/ota", []() {
    server.send_P(200, "text/html", OTA_HTML);
  });
  server.on("/ota-version", handleOtaVersion);
  server.on("/ota-status", handleOTAStatus);
  ioManager.begin();
  Serial.println(ioManager.get(0).pcf);
  Serial.println(ioManager.get(0).pin);
  
  Serial.println(ioManager.get(15).pcf);
  Serial.println(ioManager.get(15).pin);
  
  Serial.println(ioManager.get(63).pcf);
  Serial.println(ioManager.get(63).pin);
  server.begin();
  Serial.println("HTTP server started");
}

// =========================
// Loop
// =========================
void loop() {
  server.handleClient();
}
