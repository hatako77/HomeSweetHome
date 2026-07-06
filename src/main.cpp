#include <WiFi.h>
#include <LittleFS.h>
#include "Core/Version.h"
#include "IO/IOManager.h"
#include "IO/IOStorage.h"
#include "OTA/OTAService.h"
#include "Web/WebServerService.h"
// =========================
// WiFi
// =========================
const char* ssid = "Ali TT";
const char* password = "2150068486";
// =========================
extern OTAService ota;
// =========================
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
    ota.setVersionURL(
        "https://github.com/hatako77/HomeSweetHome/releases/latest/download/version.json"
    );

    if (!LittleFS.begin(true))
    {
        Serial.println("LittleFS mount failed");
    }
    ioManager.begin();
    IOStorage::load();
    web.begin();
    Serial.println("System Ready");
}

void loop()
{
    ioManager.update();
    web.update();
}
