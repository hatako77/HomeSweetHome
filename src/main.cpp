//======================== src/main.cpp ========================

#include <Arduino.h>
#include <WiFi.h>
#include <LittleFS.h>

#include "webserver.h"
#include "ota.h"
#include "version.h"

const char* ssid="YOUR_WIFI";
const char* password="YOUR_PASSWORD";

void setup()
{
    Serial.begin(115200);

    if(!LittleFS.begin(true))
    {
        Serial.println("LittleFS Error");
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid,password);

    while(WiFi.status()!=WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.print("IP : ");
    Serial.println(WiFi.localIP());

    otaInit();

    webserverInit();
}

void loop()
{
}
