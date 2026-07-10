#ifndef OTA_SERVICE_H
#define OTA_SERVICE_H

#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>
#include <ArduinoJson.h>

struct OTAStatus
{
    bool running = false;
    bool finished = false;
    bool success = false;

    uint32_t downloaded = 0;
    uint32_t total = 0;

    uint8_t percent = 0;

    float speedKB = 0;

    uint32_t eta = 0;

    String state = "Idle";
    String error = "";
};
class OTAService {
public:
    bool checkForUpdate();
    bool updateFirmware();

    void startCheck();
    void startUpdate();

    static void checkTask(void* parameter);
    static void updateTask(void* parameter);

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
