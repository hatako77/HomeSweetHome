#include <WiFi.h>
#include "Core/Version.h"
#include "IO/IOManager.h"
#include "OTA/OTAService.h"
#include "Web/WebServerService.h"
#include "Core/DemoData.h"
#include <LittleFS.h>
#include "Repositories/RoomRepository.h"
// =========================
// WiFi
// =========================
const char* ssid = "Ali TT";
const char* password = "2150068486";
// =========================
OTAService ota;
// =========================
void setup()
{
    Serial.begin(115200);

    if (!LittleFS.begin(true))
    {
        Serial.println("LittleFS Mount Failed");
        while (true);
    }

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
    roomRepository.begin();
    ioManager.begin();
    DemoData::create();
    web.begin();
    Serial.println("System Ready");
}

void loop()
{
    ioManager.update();
    web.update();
}
