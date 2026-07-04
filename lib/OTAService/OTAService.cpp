#include "OTAService.h"

void OTAService::setVersionURL(const String& url) {
    versionURL = url;
}

void OTAService::setCurrentVersion(const String& version) {
    currentVersion = version;
}

// مقایسه نسخه ساده (1.0.1 > 1.0.0)
bool OTAService::isNewVersion(String remote, String current) {
    return remote != current;
}

bool OTAService::checkForUpdate() {

    if (WiFi.status() != WL_CONNECTED) return false;

    HTTPClient http;
    http.begin(versionURL);

    int code = http.GET();
    if (code != 200) {
        http.end();
        return false;
    }

    String payload = http.getString();
    http.end();

    StaticJsonDocument<512> doc;
    DeserializationError err = deserializeJson(doc, payload);

    if (err) return false;

    remoteVersion = doc["version"].as<String>();
    firmwareURL = doc["url"].as<String>();

    return isNewVersion(remoteVersion, currentVersion);
}

bool OTAService::downloadAndUpdate(const String& url) {

    HTTPClient http;
    http.begin(url);

    int code = http.GET();
    if (code != 200) {
        http.end();
        return false;
    }

    int len = http.getSize();

    if (!Update.begin(len)) {
        http.end();
        return false;
    }

    WiFiClient* stream = http.getStreamPtr();

    size_t written = Update.writeStream(*stream);

    if (written != len) {
        http.end();
        return false;
    }

    if (!Update.end()) {
        http.end();
        return false;
    }

    return Update.isFinished();
}

bool OTAService::updateFirmware() {

    if (firmwareURL == "") return false;

    return downloadAndUpdate(firmwareURL);
}
