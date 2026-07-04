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
    
    // آدرس فایل version.txt در GitHub Releases
    String url = "https://raw.githubusercontent.com/hatako77/HomeSweetHome/main/release/version.txt";
    // یا اگر از GitHub Releases استفاده می‌کنید:
    // String url = "https://api.github.com/repos/hatako77/HomeSweetHome/releases/latest";
    
    HTTPClient http;
    http.begin(url);
    http.setTimeout(5000);
    
    int httpCode = http.GET();
    if (httpCode == 200) {
        m_latestVersion = http.getString();
        m_latestVersion.trim();
        m_progress = 30;
        
        // مقایسه نسخه‌ها
        if (m_latestVersion != currentVersion) {
            m_updateAvailable = true;
            m_status = "New version available: " + m_latestVersion;
            m_progress = 40;
            return true;
        } else {
            m_updateAvailable = false;
            m_status = "Device is up to date";
            m_progress = 100;
            return false;
        }
    } else {
        m_status = "Failed to check update: " + String(httpCode);
        m_progress = 100;
        http.end();
        return false;
    }
    http.end();
}

bool OTAHandler::startUpdate(const String& url) {
    m_status = "Downloading firmware...";
    m_progress = 10;
    
    HTTPClient http;
    http.begin(url);
    http.setTimeout(30000);
    
    int httpCode = http.GET();
    if (httpCode != 200) {
        m_status = "Download failed: " + String(httpCode);
        m_progress = 100;
        http.end();
        return false;
    }
    
    int contentLength = http.getSize();
    if (contentLength <= 0) {
        m_status = "Invalid content size";
        m_progress = 100;
        http.end();
        return false;
    }
    
    m_status = "Starting OTA update...";
    m_progress = 20;
    
    if (!Update.begin(contentLength)) {
        m_status = "OTA begin failed: " + String(Update.getError());
        m_progress = 100;
        http.end();
        return false;
    }
    
    WiFiClient* stream = http.getStreamPtr();
    size_t written = 0;
    uint8_t buff[1024];
    
    while (http.connected() && written < contentLength) {
        size_t available = stream->available();
        if (available > 0) {
            size_t toRead = min(available, sizeof(buff));
            size_t read = stream->readBytes(buff, toRead);
            
            if (read > 0) {
                if (Update.write(buff, read) != read) {
                    m_status = "Write error";
                    m_progress = 100;
                    http.end();
                    return false;
                }
                written += read;
                m_progress = 20 + (70 * written / contentLength);
                m_status = "Updating... " + String(100 * written / contentLength) + "%";
            }
        }
        delay(1);
    }
    
    http.end();
    
    if (!Update.end(true)) {
        m_status = "Update failed: " + String(Update.getError());
        m_progress = 100;
        return false;
    }
    
    m_status = "Update successful! Restarting...";
    m_progress = 100;
    return true;
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
