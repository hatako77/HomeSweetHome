#ifndef OTA_HANDLER_H
#define OTA_HANDLER_H

#include <Arduino.h>
#include <WiFiClient.h>
#include <HTTPClient.h>
#include <Update.h>

class OTAHandler {
public:
    OTAHandler();
    bool checkForUpdate(const String& currentVersion);
    bool startUpdate(const String& url);
    String getLatestVersion();
    int getProgress();
    String getStatus();
    bool isUpdateAvailable();

private:
    String m_latestVersion;
    bool m_updateAvailable;
    int m_progress;
    String m_status;
};

#endif
