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
