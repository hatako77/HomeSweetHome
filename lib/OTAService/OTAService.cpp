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
    Serial.print("versionURL: ");
    Serial.println(versionURL);
    int code = http.GET();
    Serial.print("Code: ");
    Serial.println(code);
    if (code != 200) {
        http.end();
        return false;
    }

    String payload = http.getString();
    http.end();

    JsonDocument doc;
    
    DeserializationError err = deserializeJson(doc, payload);
    
    if (err) {
        Serial.print("JSON Error: ");
        Serial.println(err.c_str());
        return false;
    }

    remoteVersion = doc["version"].as<String>();
    firmwareURL = doc["url"].as<String>();
    Serial.print("url: ");
    Serial.println(firmwareURL);

    Serial.println("----- OTA VERSION CHECK -----");
    Serial.print("Current: ");
    Serial.println(currentVersion);
    Serial.print("Remote: ");
    Serial.println(remoteVersion);
    Serial.println("-----------------------------");
    return isNewVersion(remoteVersion, currentVersion);
}

bool OTAService::downloadAndUpdate(const String& url) {

    HTTPClient http;
    http.begin(url);
    Serial.print("ur;: ");
    Serial.println(url);
    int code = http.GET();
    Serial.print("Code: ");
    Serial.println(code);

    if (code != HTTP_CODE_OK) {
        http.end();
        return false;
    }

    int len = http.getSize();
    Serial.print("len: ");
    Serial.println(len);

    if (!Update.begin(len)) {
        http.end();
        return false;
    }

    WiFiClient* stream = http.getStreamPtr();

    size_t written = 0;
    uint8_t buff[128];

    while (http.connected() && (len > 0 || len == -1)) {

        size_t size = stream->available();
        if (size) {
            int c = stream->readBytes(buff, ((size > sizeof(buff)) ? sizeof(buff) : size));

            Update.write(buff, c);
            written += c;

            if (len > 0) len -= c;

            yield();   // 💥 مهم
        }

        delay(1);
    }

    if (!Update.end()) {
        http.end();
        return false;
    }

    http.end();

    return Update.isFinished();
}
bool OTAService::updateFirmware() {

    if (firmwareURL == "") return false;

    return downloadAndUpdate(firmwareURL);
}
