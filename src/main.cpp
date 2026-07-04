// ===== روت‌های سرور =====
server.addHandler(&events);

// صفحه اصلی
server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("📄 Root page requested");
    request->send(200, "text/html", INDEX_HTML);
});

// صفحه OTA
server.on("/ota", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("📄 OTA page requested");
    request->send(200, "text/html", OTA_HTML);
});

// API: دریافت نسخه فعلی
server.on("/api/version", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("📊 Version API called");
    String json = "{\"version\":\"" + getVersion() + "\"}";
    request->send(200, "application/json", json);
});

// API: بررسی نسخه جدید (با دیباگ)
server.on("/api/check-update", HTTP_GET, [](AsyncWebServerRequest* request) {
    Serial.println("🔍 Check update API called!");  // ← این خط را اضافه کنید
    
    String currentVersion = getVersion();
    Serial.println("📦 Current version: " + currentVersion);
    
    String latestVersion = checkLatestVersion();
    Serial.println("📦 Latest version: " + latestVersion);
    
    bool hasUpdate = (latestVersion != "" && latestVersion != currentVersion);
    Serial.println("🔍 Has update: " + String(hasUpdate ? "YES" : "NO"));
    
    String json = "{";
    json += "\"current\":\"" + currentVersion + "\",";
    json += "\"latest\":\"" + latestVersion + "\",";
    json += "\"updateAvailable\":" + String(hasUpdate ? "true" : "false");
    json += "}";
    request->send(200, "application/json", json);
});

// API: شروع به‌روزرسانی
server.on("/api/start-update", HTTP_POST, [](AsyncWebServerRequest* request) {
    Serial.println("🚀 Start update API called!");
    // ... بقیه کد
});
