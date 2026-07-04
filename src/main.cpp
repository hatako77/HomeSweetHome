#include <Arduino.h>
#include <WiFi.h>

#include "config.h"
#include "version.h"
#include "web.h"

Web web;

//------------------------------------------------------------

void connectWiFi()
{
    WiFi.mode(WIFI_STA);

    Serial.println();
    Serial.println("====================================");
    Serial.println(FIRMWARE_NAME);
    Serial.print("Version : ");
    Serial.println(FIRMWARE_VERSION);
    Serial.println("====================================");

    Serial.print("Connecting to WiFi");

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    uint32_t start = millis();

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");

        delay(500);

        if (millis() - start > 30000)
        {
            Serial.println();
            Serial.println("WiFi Timeout");

            ESP.restart();
        }
    }

    Serial.println();
    Serial.println("WiFi Connected");
    Serial.print("IP Address : ");
    Serial.println(WiFi.localIP());
}

//------------------------------------------------------------

void setup()
{
    Serial.begin(115200);

    delay(1000);

    connectWiFi();

    web.begin();

    Serial.println();
    Serial.println("HTTP Server Started");
}

//------------------------------------------------------------

void loop()
{
    web.handle();

    delay(1);
}
