#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>
#include "version.h"
#include "webpage.h"
#include "ota_handler.h"

// ===== تنظیمات WiFi =====
const char* WIFI_SSID = "Ali TT";
const char* WIFI_PASSWORD = "2150068486";

// ===== سرور وب =====
AsyncWebServer server(80);
AsyncEventSource events("/events");

// ===== OTA Handler =====
OTAHandler otaHandler;

// ===== مدیریت نسخه =====
String getVersion() {
    return String(FIRMWARE_VERSION);
}

// ===== ارسال پیشرفت OTA =====
void sendOTAProgress(int progress, const String& status) {
    String json = "{";
    json += "\"type\":\"progress\",";
    json += "\"progress\":" + String(progress) + ",";
    json += "\"status\":\"" + status + "\"";
    if (progress >= 100) {
        json += ",\"type\":\"complete\"";
        json += ",\"message\":\"Update completed successfully!\"";
    }
    json += "}";
    events.send(json.c_str(), "message", millis());
}

// ===== بررسی نسخه جدید از version.txt (با شکستن کش) =====
String checkLatestVersion() {
    Serial.println("🌐 Checking version.txt from GitHub...");
    
    // پارامتر تصادفی برای شکستن کش
    String randomParam = "?t=" + String(random(100000, 999999));
    String url = "https://raw.githubusercontent.com/hatako77/HomeSweetHome/main/version.txt" + randomParam;
    
    HTTPClient http;
    http.begin(url);
    http.setTimeout(5000);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    
    // هدرهای no-cache
    http.addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    http.addHeader("Pragma", "no-cache");
    http.addHeader("Expires", "0");
    
    int httpCode = http.GET();
    Serial.println("📡 HTTP Code: " + String(httpCode));
    
    String latestVersion = "";
    if (httpCode == 200) {
        latestVersion = http.getString();
        latestVersion.trim();
        Serial.println("✅ Version from GitHub: [" + latestVersion + "]");
    } else {
        Serial.println("❌ Failed to get version: " + String(httpCode));
    }
    http.end();
    return latestVersion;
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
    Serial.print("📶 Connecting to WiFi");
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
    
    // ===== روت‌های سرور =====
    server.addHandler(&events);
    
    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(200, "text/html", INDEX_HTML);
    });
    
    server.on("/ota", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(200, "text/html", OTA_HTML);
    });
    
    server.on("/api/version", HTTP_GET, [](AsyncWebServerRequest* request) {
        String json = "{\"version\":\"" + getVersion() + "\"}";
        request->send(200, "application/json", json);
    });
    
    server.on("/api/check-update", HTTP_GET, [](AsyncWebServerRequest* request) {
        Serial.println("🔍 Check update API called!");
        
        String currentVersion = getVersion();
        String latestVersion = checkLatestVersion();
        
        bool hasUpdate = (latestVersion != "" && latestVersion != currentVersion);
        
        String json = "{";
        json += "\"current\":\"" + currentVersion + "\",";
        json += "\"latest\":\"" + latestVersion + "\",";
        json += "\"updateAvailable\":" + String(hasUpdate ? "true" : "false");
        json += "}";
        request->send(200, "application/json", json);
    });
    
    server.on("/api/start-update", HTTP_POST, [](AsyncWebServerRequest* request) {
        String latestVersion = checkLatestVersion();
        String currentVersion = getVersion();
        
        if (latestVersion == "" || latestVersion == currentVersion) {
            request->send(400, "application/json", "{\"success\":false,\"error\":\"No update available\"}");
            return;
        }
        
        String url = "https://github.com/hatako77/HomeSweetHome/releases/download/v" + latestVersion + "/firmware.bin";
        bool success = otaHandler.startUpdate(url);
        
        String json = "{\"success\":" + String(success ? "true" : "false") + "}";
        request->send(200, "application/json", json);
        
        if (success) {
            delay(1000);
            ESP.restart();
        }
    });
    
    server.begin();
    Serial.println("✅ Web Server started on port 80");
    Serial.println("=================================\n");
    Serial.println("🚀 System Ready!");
}

void loop() {
    static int lastProgress = -1;
    int progress = otaHandler.getProgress();
    if (progress != lastProgress) {
        lastProgress = progress;
        sendOTAProgress(progress, otaHandler.getStatus());
    }
    delay(100);
}
