#ifndef OTA_HANDLER_H
#define OTA_HANDLER_H

#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>
#include <functional>

enum class OTAStatus {
    IDLE,
    CHECKING,
    DOWNLOADING,
    UPDATING,
    COMPLETE,
    ERROR
};

class OTAHandler {
public:
    using ProgressCallback = std::function<void(int progress, const String& status)>;
    using CompleteCallback = std::function<void(bool success, const String& message)>;

    OTAHandler();
    ~OTAHandler();

    bool begin();
    bool checkForUpdate(const String& currentVersion);
    bool startUpdate(const String& url);
    bool processManualUpdate(const uint8_t* data, size_t len, bool final);
    void reset();
    
    // Getters
    bool isUpdateAvailable() const { return m_updateAvailable; }
    String getLatestVersion() const { return m_latestVersion; }
    OTAStatus getStatus() const { return m_status; }
    int getProgress() const { return m_progress; }
    String getStatusMessage() const { return m_statusMessage; }
    
    // Callbacks
    void setProgressCallback(ProgressCallback callback) { m_progressCallback = callback; }
    void setCompleteCallback(CompleteCallback callback) { m_completeCallback = callback; }

private:
    bool m_updateAvailable;
    String m_latestVersion;
    String m_updateURL;
    OTAStatus m_status;
    int m_progress;
    String m_statusMessage;
    
    ProgressCallback m_progressCallback;
    CompleteCallback m_completeCallback;
    
    size_t m_totalBytes;
    size_t m_writtenBytes;
    
    bool connectToWiFi();
    void setStatus(OTAStatus status, const String& message, int progress = -1);
    void sendProgress(int progress, const String& status);
    bool compareVersions(const String& v1, const String& v2);
    String getVersionFromURL(const String& url);
};

#endif
