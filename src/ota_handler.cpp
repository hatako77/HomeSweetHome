#include "ota_handler.h"
#include "config.h"

OTAHandler::OTAHandler() 
    : m_updateAvailable(false)
    , m_latestVersion("")
    , m_updateURL("")
    , m_status(OTAStatus::IDLE)
    , m_progress(0)
    , m_statusMessage("Ready")
    , m_totalBytes(0)
    , m_writtenBytes(0) {
}

OTAHandler::~OTAHandler() {
}

bool OTAHandler::begin() {
    if (!connectToWiFi()) {
        setStatus(OTAStatus::ERROR, "WiFi connection failed");
        return false;
    }
    
    setStatus(OTAStatus::IDLE, "OTA Handler ready");
    DEBUG_PRINTLN("OTA Handler initialized successfully");
    return true;
}

bool OTAHandler::connectToWiFi() {
    if (WiFi.status() == WL_CONNECTED) {
        return true;
    }
    
    DEBUG_PRINT("Connecting to WiFi");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < WIFI_TIMEOUT) {
        delay(500);
        DEBUG_PRINT(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        DEBUG_PRINTLN("\n✅ WiFi Connected!");
        DEBUG_PRINT("IP Address: ");
        DEBUG_PRINTLN(WiFi.localIP());
        return true;
    }
    
    DEBUG_PRINTLN("\n❌ WiFi Connection Failed!");
    return false;
}

bool OTAHandler::checkForUpdate(const String& currentVersion) {
    if (!connectToWiFi()) {
        setStatus(OTAStatus::ERROR, "WiFi connection lost");
        return false;
    }
    
    setStatus(OTAStatus::CHECKING, "Checking for updates...", 10);
    
    HTTPClient http;
    String url = "http://" + String(OTA_HOST) + OTA_PATH + OTA_VERSION_FILE;
    http.begin(url);
    http.setTimeout(5000);
    http.addHeader("Cache-Control", "no-cache");
    
    int httpCode = http.GET();
    if (httpCode == 200) {
        m_latestVersion = http.getString();
        m_latestVersion.trim();
        
        setStatus(OTAStatus::CHECKING, "Version checked: " + m_latestVersion, 30);
        
        m_updateAvailable = compareVersions(currentVersion, m_latestVersion);
        
        if (m_updateAvailable) {
            m_updateURL = "http://" + String(OTA_HOST) + OTA_PATH + OTA_FIRMWARE_FILE;
            setStatus(OTAStatus::CHECKING, "New version available: " + m_latestVersion, 40);
            DEBUG_PRINTLN("✅ Update available!");
        } else {
            setStatus(OTAStatus::IDLE, "Device is up to date", 100);
            DEBUG_PRINTLN("✅ Device is up to date");
        }
    } else {
        String error = "HTTP error: " + String(httpCode);
        setStatus(OTAStatus::ERROR, error, 100);
        DEBUG_PRINTLN("❌ " + error);
        http.end();
        return false;
    }
    
    http.end();
    return m_updateAvailable;
}

bool OTAHandler::startUpdate(const String& url) {
    if (!connectToWiFi()) {
        setStatus(OTAStatus::ERROR, "WiFi connection lost");
        return false;
    }
    
    if (!m_updateAvailable) {
        setStatus(OTAStatus::ERROR, "No update available");
        return false;
    }
    
    setStatus(OTAStatus::DOWNLOADING, "Downloading firmware...", 10);
    
    HTTPClient http;
    http.begin(url);
    http.setTimeout(30000);
    http.addHeader("Cache-Control", "no-cache");
    
    int httpCode = http.GET();
    if (httpCode != 200) {
        String error = "Download failed: " + String(httpCode);
        setStatus(OTAStatus::ERROR, error, 100);
        DEBUG_PRINTLN("❌ " + error);
        http.end();
        return false;
    }
    
    int contentLength = http.getSize();
    if (contentLength <= 0 || contentLength > MAX_SKETCH_SIZE) {
        String error = "Invalid content size: " + String(contentLength);
        setStatus(OTAStatus::ERROR, error, 100);
        DEBUG_PRINTLN("❌ " + error);
        http.end();
        return false;
    }
    
    setStatus(OTAStatus::UPDATING, "Starting OTA update...", 20);
    
    if (!Update.begin(contentLength)) {
        String error = "OTA begin failed: " + String(Update.getError());
        setStatus(OTAStatus::ERROR, error, 100);
        DEBUG_PRINTLN("❌ " + error);
        http.end();
        return false;
    }
    
    WiFiClient* stream = http.getStreamPtr();
    m_totalBytes = contentLength;
    m_writtenBytes = 0;
    
    uint8_t buff[1024];
    size_t buffSize = sizeof(buff);
    
    while (http.connected() && m_writtenBytes < m_totalBytes) {
        size_t available = stream->available();
        if (available > 0) {
            size_t toRead = min(available, buffSize);
            size_t read = stream->readBytes(buff, toRead);
            
            if (read > 0) {
                if (Update.write(buff, read) != read) {
                    String error = "Write error";
                    setStatus(OTAStatus::ERROR, error, 100);
                    http.end();
                    return false;
                }
                m_writtenBytes += read;
                int progress = 20 + (70 * m_writtenBytes / m_totalBytes);
                String status = "Updating... " + String(100 * m_writtenBytes / m_totalBytes) + "%";
                setStatus(OTAStatus::UPDATING, status, progress);
            }
        }
        delay(1);
    }
    
    http.end();
    
    if (m_writtenBytes != m_totalBytes) {
        String error = "Incomplete download: " + String(m_writtenBytes) + "/" + String(m_totalBytes);
        setStatus(OTAStatus::ERROR, error, 100);
        DEBUG_PRINTLN("❌ " + error);
        return false;
    }
    
    if (!Update.end(true)) {
        String error = "Update end failed: " + String(Update.getError());
        setStatus(OTAStatus::ERROR, error, 100);
        DEBUG_PRINTLN("❌ " + error);
        return false;
    }
    
    setStatus(OTAStatus::COMPLETE, "Update successful! Restarting...", 100);
    DEBUG_PRINTLN("✅ Update successful!");
    
    if (m_completeCallback) {
        m_completeCallback(true, "Update successful!");
    }
    
    return true;
}

bool OTAHandler::processManualUpdate(const uint8_t* data, size_t len, bool final) {
    static bool updateStarted = false;
    static size_t totalSize = 0;
    
    if (!updateStarted) {
        totalSize = 0;
        updateStarted = true;
        
        if (!Update.begin(MAX_SKETCH_SIZE)) {
            String error = "OTA begin failed: " + String(Update.getError());
            setStatus(OTAStatus::ERROR, error, 100);
            DEBUG_PRINTLN("❌ " + error);
            updateStarted = false;
            return false;
        }
        
        setStatus(OTAStatus::UPDATING, "Starting manual update...", 10);
        DEBUG_PRINTLN("📤 Manual update started");
    }
    
    // اصلاح: تبدیل const uint8_t* به uint8_t*
    if (Update.write((uint8_t*)data, len) != len) {
        String error = "Write error at offset " + String(totalSize);
        setStatus(OTAStatus::ERROR, error, 100);
        DEBUG_PRINTLN("❌ " + error);
        updateStarted = false;
        Update.abort();
        return false;
    }
    
    totalSize += len;
    int progress = 10 + (70 * totalSize / MAX_SKETCH_SIZE);
    if (progress > 90) progress = 90;
    String status = "Uploading... " + String(100 * totalSize / MAX_SKETCH_SIZE) + "%";
    setStatus(OTAStatus::UPDATING, status, progress);
    
    if (final) {
        if (!Update.end(true)) {
            String error = "Update end failed: " + String(Update.getError());
            setStatus(OTAStatus::ERROR, error, 100);
            DEBUG_PRINTLN("❌ " + error);
            updateStarted = false;
            return false;
        }
        
        setStatus(OTAStatus::COMPLETE, "Update successful! Restarting...", 100);
        DEBUG_PRINTLN("✅ Manual update successful!");
        
        if (m_completeCallback) {
            m_completeCallback(true, "Manual update successful!");
        }
        
        updateStarted = false;
        return true;
    }
    
    return true;
}

void OTAHandler::reset() {
    m_updateAvailable = false;
    m_latestVersion = "";
    m_updateURL = "";
    m_status = OTAStatus::IDLE;
    m_progress = 0;
    m_statusMessage = "Ready";
    m_totalBytes = 0;
    m_writtenBytes = 0;
    Update.abort();
}

void OTAHandler::setStatus(OTAStatus status, const String& message, int progress) {
    m_status = status;
    m_statusMessage = message;
    if (progress >= 0) {
        m_progress = progress;
    }
    sendProgress(m_progress, message);
}

void OTAHandler::sendProgress(int progress, const String& status) {
    if (m_progressCallback) {
        m_progressCallback(progress, status);
    }
    // اصلاح: استفاده از DEBUG_PRINT به جای DEBUG_PRINTF
    DEBUG_PRINT("[");
    DEBUG_PRINT(progress);
    DEBUG_PRINT("%] ");
    DEBUG_PRINTLN(status);
}

bool OTAHandler::compareVersions(const String& v1, const String& v2) {
    if (v1 == v2) return false;
    
    int v1Major = 0, v1Minor = 0, v1Patch = 0, v1Build = 0;
    int v2Major = 0, v2Minor = 0, v2Patch = 0, v2Build = 0;
    
    sscanf(v1.c_str(), "%d.%d.%d.%d", &v1Major, &v1Minor, &v1Patch, &v1Build);
    sscanf(v2.c_str(), "%d.%d.%d.%d", &v2Major, &v2Minor, &v2Patch, &v2Build);
    
    if (v1Major != v2Major) return v2Major > v1Major;
    if (v1Minor != v2Minor) return v2Minor > v1Minor;
    if (v1Patch != v2Patch) return v2Patch > v1Patch;
    return v2Build > v1Build;
}
