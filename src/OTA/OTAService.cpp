#include "Web/WebSocketService.h"
extern WebSocketService websocket;

void OTAService::setVersionURL(const String& url) {
    versionURL = url;
}

void OTAService::startCheck() {
    xTaskCreate( checkTask, "ota_check",
        8192, this, 1, nullptr);        
}
void OTAService::startUpdate() {
    xTaskCreate(updateTask, "ota_update",
        12288, this, 1, nullptr);       
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
    String url = versionURL + "?ts=" + String(esp_random());
    HTTPClient http;
    http.setReuse(false);
    http.useHTTP10(true);
    http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
    http.addHeader("Cache-Control", "no-cache");
    http.addHeader("Pragma", "no-cache");
    http.begin(url);
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
    
    Serial.println("========== JSON ==========");
    Serial.println(payload);
    Serial.println("==========================");
    Serial.print("Requested URL: ");
    Serial.println(url);
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

bool OTAService::downloadAndUpdate(const String& url)
{
    status.running = true;
    status.finished = false;
    status.success = false;

    status.downloaded = 0;
    status.total = 0;
    status.percent = 0;

    status.speedKB = 0;
    status.eta = 0;

    status.error = "";
    status.state = "Connecting";

    HTTPClient http;

    http.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);

    http.begin(url);

    int code = http.GET();

    if(code != HTTP_CODE_OK)
    {
        status.error = "HTTP Error " + String(code);
        status.running = false;
        status.finished = true;
        status.success = false;
        return false;
    }

    int total = http.getSize();

    status.total = total;

    if(!Update.begin(total))
    {
        status.error = "Not enough flash";
        status.running = false;
        http.end();
        status.finished = true;
        status.success = false;
        return false;
    }

    WiFiClient *stream = http.getStreamPtr();

    uint8_t buffer[1024];

    uint32_t lastMillis = millis();
    uint32_t lastBytes = 0;

    status.state = "Downloading";

    while(http.connected() && status.downloaded < status.total)
    {
        size_t available = stream->available();

        if(available)
        {
            int len = stream->readBytes(buffer,
                                       min((size_t)1024,available));

            if(len>0)
            {
                size_t written = Update.write(buffer, len);

                    if (written != (size_t)len)
                    {
                        status.error = Update.errorString();
                    
                        status.running = false;
                    
                        http.end();
                        status.finished = true;
                        status.success = false;
                        return false;
                    }

                status.downloaded += len;

                status.percent =
                    (status.downloaded*100)/status.total;
            }
        }

        if(millis()-lastMillis>1000)
        {
            uint32_t diff =
                status.downloaded-lastBytes;

            status.speedKB = diff/1024.0;

            if(status.speedKB>0)
            {
                status.eta =
                ((status.total-status.downloaded)/1024.0)
                /status.speedKB;
            }

            lastBytes=status.downloaded;

            lastMillis=millis();
        }

        delay(1);
    }

    status.state="Verifying";

    if(!Update.end())
    {
        status.error="Verification failed";

        status.running=false;

        http.end();
        status.finished = true;
        status.success = false;
        return false;
    }

    if(!Update.isFinished())
    {
        status.error="Update not finished";

        status.running=false;

        http.end();
        status.finished = true;
        status.success = false;
        return false;
    }

    http.end();

    status.percent=100;

    status.success=true;

    status.finished=true;

    status.running=false;
    status.speedKB = 0;
    status.eta = 0;
    status.state="Completed";

    return true;
}

bool OTAService::updateFirmware() {

    if (firmwareURL == "") return false;

    return downloadAndUpdate(firmwareURL);
}
void OTAService::checkTask(void* parameter)
{
    OTAService* self =
        static_cast<OTAService*>(parameter);

    self->status.running = true;
    self->status.finished = false;
    self->status.success = false;

    self->status.state = "Checking";

    websocket.notifyOTA(self->status);

    bool available =
        self->checkForUpdate();

    self->status.running = false;
    self->status.finished = true;
    self->status.success = available;

    self->status.state =
        available ? "Update Available"
                  : "Up To Date";

    websocket.notifyOTA(self->status);

    vTaskDelete(nullptr);
}
void OTAService::updateTask(void* parameter)
{
    OTAService* self =
        static_cast<OTAService*>(parameter);

    self->updateFirmware();

    websocket.notifyOTA(
        self->status
    );

    if(self->status.success)
    {
        delay(2000);

        ESP.restart();
    }

    vTaskDelete(nullptr);
}
