#ifndef OTA_SERVICE_H
#define OTA_SERVICE_H
#include "OTA/OTAStatus.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>
#include <ArduinoJson.h>

class OTAService {
public:
    bool checkForUpdate();
    bool updateFirmware();

    void startCheck();
    void startUpdate();

    static void checkTask(void* parameter);
    static void updateTask(void* parameter);
    void setVersionURL(const String& url);
    void setCurrentVersion(const String& version);
    OTAStatus& getStatus()
    {
        return status;
    }
private:
    String versionURL;
    String currentVersion;
    OTAStatus status;
    String remoteVersion;
    String firmwareURL;

    bool downloadAndUpdate(const String& url);
    bool isNewVersion(String remote, String current);
};

#endif
