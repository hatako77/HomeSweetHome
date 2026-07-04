#include "ota_handler.h"

OTAHandler::OTAHandler() {
    m_latestVersion = "";
    m_updateAvailable = false;
    m_progress = 0;
    m_status = "Idle";
}

bool OTAHandler::checkForUpdate(const String& currentVersion) {
    m_status = "Checking for updates...";
    m_progress = 10;
    
    String url = "https://raw.githubusercontent.com/hatako77/HomeSweetHome/main/version.txt";
    
    HTTPClient http;
    http.begin(url);
    http.setTimeout(5000);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    
    int httpCode = http.GET();
    if (httpCode == 200) {
        m_latestVersion = http.getString();
        m_latestVersion.trim();
        m_progress = 30;
        
        // دیباگ کامل
        Serial.println("🔍 DEBUG: Current version: [" + currentVersion + "]");
        Serial.println("🔍 DEBUG: Latest version:  [" + m_latestVersion + "]");
        Serial.println("🔍 DEBUG: Compare result:  " + String(m_latestVersion != currentVersion));
        
        if (m_latestVersion != currentVersion) {
            m_updateAvailable = true;
            m_status = "New version available: " + m_latestVersion;
            m_progress = 40;
            Serial.println("✅ Update available!");
            return true;
        } else {
            m_updateAvailable = false;
            m_status = "Device is up to date";
            m_progress = 100;
            Serial.println("✅ Device is up to date");
            return false;
        }
    } else {
        m_status = "Failed to check update: " + String(httpCode);
        m_progress = 100;
        Serial.println("❌ Failed to check update: " + String(httpCode));
        http.end();
        return false;
    }
    http.end();
    return false;
}

bool OTAHandler::startUpdate(const String& url) {
    // ... کد قبلی
    // برای دیباگ فعلاً نیازی به این بخش نیست
    return false;
}

String OTAHandler::getLatestVersion() {
    return m_latestVersion;
}

int OTAHandler::getProgress() {
    return m_progress;
}

String OTAHandler::getStatus() {
    return m_status;
}

bool OTAHandler::isUpdateAvailable() {
    return m_updateAvailable;
}
