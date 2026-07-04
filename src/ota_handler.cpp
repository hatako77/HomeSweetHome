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
    
    String url = "https://raw.githubusercontent.com/hatako77/HomeSweetHome/main/version.json";
    HTTPClient http;
    http.begin(url);
    http.setTimeout(5000);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.addHeader("Cache-Control", "no-cache");
    
    int httpCode = http.GET();
    if (httpCode == 200) {
        String response = http.getString();
        int start = response.indexOf("\"version\":\"") + 11;
        if (start > 10) {
            int end = response.indexOf("\"", start);
            if (end > start) {
                m_latestVersion = response.substring(start, end);
                m_latestVersion.trim();
            }
        }
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
    Serial.println("🚀 Starting OTA update...");
    Serial.println("📥 URL: " + url);
    
    m_status = "Downloading firmware...";
    m_progress = 10;
    
    HTTPClient http;
    http.begin(url);
    http.setTimeout(30000);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    
    Serial.println("📡 Sending request...");
    
    int httpCode = http.GET();
    Serial.println("📡 HTTP Code: " + String(httpCode));
    
    if (httpCode != 200) {
        String errorMsg = "Download failed: " + String(httpCode);
        m_status = errorMsg;
        m_progress = 100;
        http.end();
        Serial.println("❌ " + errorMsg);
        return false;
    }
    
    int contentLength = http.getSize();
    Serial.println("📦 Content length: " + String(contentLength) + " bytes (" + String(contentLength / 1024) + " KB)");
    
    if (contentLength <= 0) {
        m_status = "Invalid content size: " + String(contentLength);
        m_progress = 100;
        http.end();
        return false;
    }
    
    int freeSpace = ESP.getFreeSketchSpace();
    Serial.println("💾 Free sketch space: " + String(freeSpace) + " bytes");
    
    if (contentLength > (freeSpace - 0x1000)) {
        m_status = "Not enough space";
        m_progress = 100;
        http.end();
        return false;
    }
    
    m_status = "Starting OTA update... Size: " + String(contentLength / 1024) + " KB";
    m_progress = 20;
    Serial.println("📝 Starting Update.begin()...");
    
    if (!Update.begin(contentLength)) {
        String error = "OTA begin failed: " + String(Update.getError());
        m_status = error;
        m_progress = 100;
        http.end();
        Serial.println("❌ " + error);
        return false;
    }
    Serial.println("✅ Update.begin() successful");
    
    WiFiClient* stream = http.getStreamPtr();
    size_t written = 0;
    uint8_t buff[1024];
    unsigned long lastYield = millis();
    int emptyReads = 0;  // شمارنده برای تاخیر بیشتر
    
    while (http.connected() && written < contentLength) {
        // 🔥 مهم: هر بار که داده‌ای نیست، تاخیر بیشتری بذار
        if (stream->available() == 0) {
            emptyReads++;
            if (emptyReads > 10) {
                delay(5);  // تاخیر بیشتر
                emptyReads = 0;
            } else {
                delay(1);
            }
            continue;
        }
        emptyReads = 0;
        
        size_t toRead = min((size_t)stream->available(), sizeof(buff));
        size_t read = stream->readBytes(buff, toRead);
        
        if (read > 0) {
            size_t wrote = Update.write(buff, read);
            if (wrote != read) {
                m_status = "Write error: wrote " + String(wrote) + " of " + String(read);
                m_progress = 100;
                http.end();
                Serial.println("❌ " + m_status);
                return false;
            }
            written += wrote;
            m_progress = 20 + (70 * written / contentLength);
            m_status = "Updating... " + String(100 * written / contentLength) + "%";
            
            // 🔥 هر بار که داده نوشته میشه، yield رو صدا بزن
            yield();
        }
        
        // 🔥 هر 5 میلی‌ثانیه یکبار yield رو صدا بزن
        if (millis() - lastYield > 5) {
            yield();
            lastYield = millis();
        }
    }
    
    http.end();
    Serial.println("📦 Download complete. Written: " + String(written) + "/" + String(contentLength));
    
    if (written != contentLength) {
        m_status = "Incomplete: " + String(written) + "/" + String(contentLength);
        m_progress = 100;
        return false;
    }
    
    Serial.println("📝 Calling Update.end()...");
    if (!Update.end(true)) {
        String error = "Update end failed: " + String(Update.getError());
        m_status = error;
        m_progress = 100;
        Serial.println("❌ " + error);
        return false;
    }
    
    m_status = "Update successful! Restarting...";
    m_progress = 100;
    Serial.println("✅ " + m_status);
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
