#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "version.h"
#include "webpage.h"

// ===== تنظیمات WiFi =====
const char* WIFI_SSID = "YourWiFiSSID";
const char* WIFI_PASSWORD = "YourWiFiPassword";

// ===== سرور وب =====
AsyncWebServer server(80);

// ===== مدیریت نسخه =====
String getVersion() {
    return String(FIRMWARE_VERSION);
}

// ===== راه‌اندازی =====
void setup() {
    Serial.begin(115200);
    delay(1000);
    
    Serial.println("\n=================================");
    Serial.println("ESP32 Base Project");
    Serial.println("Version: " + getVersion());
    Serial.println("Build: " + String(BUILD_DATE) + " @ " + String(BUILD_TIME));
    Serial.println("=================================\n");
    
    // ===== اتصال به WiFi =====
    Serial.print("Connecting to WiFi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 30) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\n✅ WiFi Connected!");
        Serial.print("🌐 IP Address: ");
        Serial.println(WiFi.localIP());
    } else {
        Serial.println("\n❌ WiFi Connection Failed!");
    }
    
    // ===== راه‌اندازی سرور وب =====
    // استفاده از روش جدید (بدون _P)
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(200, "text/html", INDEX_HTML);
    });
    
    server.on("/api/version", HTTP_GET, [](AsyncWebServerRequest* request) {
        String json = "{\"version\":\"" + getVersion() + "\"}";
        request->send(200, "application/json", json);
    });
    
    server.begin();
    Serial.println("✅ Web Server started on port 80");
    Serial.println("🌐 URL: http://" + WiFi.localIP().toString());
    Serial.println("=================================\n");
    Serial.println("🚀 System Ready!");
}

void loop() {
    delay(10);
}
