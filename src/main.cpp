#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include "Version.h"
#include "OTAService.h"
// Web UI (PROGMEM)
#include "WebUI/index_html.h"
#include "WebUI/style_css.h"
#include "WebUI/app_js.h"

// ===== WiFi Config =====
const char* ssid = "Ali TT";
const char* password = "2150068486";

// ===== Web Server =====
WebServer server(80);
OTAService ota;
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
void handleRoot() {
  server.send_P(200, "text/html", INDEX_HTML);
}
void handleUpdate() {
  server.send(200, "text/plain", "Checking update...");

  bool hasUpdate = ota.checkForUpdate();

  if (!hasUpdate) {
    Serial.println("No update available");
    return;
  }

  Serial.println("New firmware found, updating...");

  bool result = ota.updateFirmware();

  if (result) {
    Serial.println("Update success, rebooting...");
    ESP.restart();
  } else {
    Serial.println("Update failed");
  }
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
  ota.setVersionURL("https://raw.githubusercontent.com/hatako77/HomeSweetHome/main/version.json");

  Serial.println("Checking OTA update...");

  if (ota.checkForUpdate()) {
    Serial.println("Update available! Installing...");

    if (ota.updateFirmware()) {
      Serial.println("Update OK. Restarting...");
      delay(2000);
      ESP.restart();
    } else {
      Serial.println("Update failed!");
    }
  }
  // Routes
  server.on("/", handleRoot);
  server.on("/style.css", handleCSS);
  server.on("/app.js", handleJS);
  server.on("/version", handleVersion);
  server.on("/relay", handleRelay);
  server.on("/update", handleUpdate);
  
  server.begin();
  Serial.println("HTTP server started");
}

// =========================
// Loop
// =========================
void loop() {
  server.handleClient();
}
