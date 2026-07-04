#ifndef OTA_SERVICE_H
#define OTA_SERVICE_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>
#include <ArduinoJson.h>

class OTAService {
public:
    bool checkForUpdate();
    bool updateFirmware();

    struct OTAStatus {
        bool running;
        bool finished;
        bool success;
    
        uint32_t downloaded;
        uint32_t total;
        uint8_t percent;
    
        float speedKB;
    
        uint32_t eta;
    
        String state;
        String error;
    };
    
void setVersionURL(const String& url);
    void setCurrentVersion(const String& version);
    String getRemoteVersion() const {
    return remoteVersion;
    }

    String getCurrentVersion() const {
    return currentVersion;
    }

private:
    String versionURL;
    String currentVersion;

    String remoteVersion;
    String firmwareURL;

    bool downloadAndUpdate(const String& url);
    bool isNewVersion(String remote, String current);
};

#endif
