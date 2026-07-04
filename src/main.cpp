#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "web_server.h"
#include "ota_handler.h"
#include "version.h"

WebServer webServer;
OTAHandler otaHandler;

bool systemInitialized = false;
unsigned long lastOTACheck = 0;

void setupSystem();

void setup() {
    Serial.begin(SERIAL_BAUD);
    delay(1000);
    
    DEBUG_PRINTLN("\n");
    DEBUG_PRINTLN("╔══════════════════════════════════════════════════════╗");
    DEBUG_PRINTLN("║              ESP32 OTA Update System                ║");
    DEBUG_PRINTLN("╚══════════════════════════════════════════════════════╝");
    DEBUG_PRINT("📦 Firmware Version: ");
    DEBUG_PRINTLN(FIRMWARE_VERSION);
    DEBUG_PRINT("📅 Build Date: ");
    DEBUG_PRINT(BUILD_DATE);
    DEBUG_PRINT(" @ ");
    DEBUG_PRINTLN(BUILD_TIME);
    DEBUG_PRINTLN("───────────────────────────────────────────────────────");
    
    setupSystem();
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        DEBUG_PRINTLN("⚠️ WiFi connection lost, attempting to reconnect...");
        WiFi.reconnect();
        int attempts = 0;
        while (WiFi.status() != WL_CONNECTED && attempts < 10) {
            delay(500);
            attempts++;
        }
        if (WiFi.status() == WL_CONNECTED) {
            DEBUG_PRINTLN("✅ WiFi reconnected!");
        }
    }
    
    if (WiFi.status() == WL_CONNECTED && systemInitialized) {
        unsigned long currentTime = millis();
        if (lastOTACheck == 0 || (currentTime - lastOTACheck) >= OTA_CHECK_INTERVAL) {
            lastOTACheck = currentTime;
            DEBUG_PRINTLN("🔍 Checking for OTA updates...");
            if (otaHandler.checkForUpdate(FIRMWARE_VERSION)) {
                DEBUG_PRINTLN("🔄 New version available: " + otaHandler.getLatestVersion());
            }
        }
    }
    
    delay(10);
}

void setupSystem() {
    DEBUG_PRINT("📶 Connecting to WiFi");
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < WIFI_TIMEOUT) {
        delay(500);
        DEBUG_PRINT(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        DEBUG_PRINTLN("\n✅ WiFi Connected!");
        DEBUG_PRINT("🌐 IP Address: ");
        DEBUG_PRINTLN(WiFi.localIP());
    } else {
        DEBUG_PRINTLN("\n❌ WiFi Connection Failed!");
    }
    
    DEBUG_PRINTLN("🔄 Initializing OTA Handler...");
    otaHandler.setProgressCallback([&](int progress, const String& status) {
        webServer.sendUpdateProgress(progress, status);
    });
    otaHandler.setCompleteCallback([&](bool success, const String& message) {
        if (success) {
            DEBUG_PRINTLN("✅ " + message);
        } else {
            DEBUG_PRINTLN("❌ " + message);
        }
    });
    
    if (!otaHandler.begin()) {
        DEBUG_PRINTLN("❌ OTA Handler initialization failed!");
    } else {
        DEBUG_PRINTLN("✅ OTA Handler initialized");
    }
    
    DEBUG_PRINTLN("🌐 Initializing Web Server...");
    webServer.setUpdateCompleteCallback([]() {
        DEBUG_PRINTLN("✅ Update complete, restarting in 5 seconds...");
    });
    
    if (!webServer.begin()) {
        DEBUG_PRINTLN("❌ Web Server initialization failed!");
    }
    
    systemInitialized = true;
    DEBUG_PRINTLN("───────────────────────────────────────────────────────");
    DEBUG_PRINTLN("🚀 System Ready!");
    DEBUG_PRINTLN("───────────────────────────────────────────────────────");
}
