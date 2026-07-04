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
    return false;
}

bool OTAHandler::startUpdate(const String& url) {
    m_status = "Downloading firmware...";
    m_progress = 10;
    
    HTTPClient http;
    http.begin(url);
    http.setTimeout(30000);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    
    int httpCode = http.GET();
    if (httpCode != 200) {
        m_status = "Download failed: " + String(httpCode);
        m_progress = 100;
        http.end();
        return false;
    }
    
    int contentLength = http.getSize();
    if (contentLength <= 0) {
        m_status = "Invalid content size: " + String(contentLength);
        m_progress = 100;
        http.end();
        return false;
    }
    
    if (contentLength > (ESP.getFreeSketchSpace() - 0x1000)) {
        m_status = "Not enough space: " + String(contentLength) + " > " + String(ESP.getFreeSketchSpace());
        m_progress = 100;
        http.end();
        return false;
    }
    
    m_status = "Starting OTA update... Size: " + String(contentLength / 1024) + " KB";
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
    unsigned long lastYield = millis();
    
    while (http.connected() && written < contentLength) {
        // 🔥 مهم: هر بار که داده‌ای وجود ندارد، کمی تاخیر بگذار
        if (stream->available() == 0) {
            delay(1);  // تاخیر کوتاه
            continue;
        }
        
        size_t toRead = min((size_t)stream->available(), sizeof(buff));
        size_t read = stream->readBytes(buff, toRead);
        
        if (read > 0) {
            size_t wrote = Update.write(buff, read);
            if (wrote != read) {
                m_status = "Write error: wrote " + String(wrote) + " of " + String(read);
                m_progress = 100;
                http.end();
                return false;
            }
            written += wrote;
            m_progress = 20 + (70 * written / contentLength);
            m_status = "Updating... " + String(100 * written / contentLength) + "%";
        }
        
        // هر 5 میلی‌ثانیه یکبار yield را صدا بزن
        if (millis() - lastYield > 5) {
            yield();
            lastYield = millis();
        }
    }
    
    http.end();
    
    if (written != contentLength) {
        m_status = "Incomplete: " + String(written) + "/" + String(contentLength);
        m_progress = 100;
        return false;
    }
    
    if (!Update.end(true)) {
        m_status = "Update end failed: " + String(Update.getError());
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
