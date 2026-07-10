#include "OTA/OTAService.h"
#include "Web/WebSocketService.h"

extern WebSocketService websocket;

void OTAService::setVersionURL(const String& url)
{
    versionURL = url;
}

void OTAService::setCurrentVersion(const String& version)
{
    currentVersion = version;
}

void OTAService::startCheck()
{
    xTaskCreate(
        checkTask,
        "ota_check",
        8192,
        this,
        1,
        nullptr
    );
}

void OTAService::startUpdate()
{
    xTaskCreate(
        updateTask,
        "ota_update",
        12288,
        this,
        1,
        nullptr
    );
}

bool OTAService::isNewVersion(String remote, String current)
{
    return remote != current;
}

bool OTAService::checkForUpdate()
{
    status.running = true;
    status.finished = false;
    status.success = false;
    status.error = "";
    status.state = "Checking";

    websocket.notifyOTA(status);

    if (WiFi.status() != WL_CONNECTED)
    {
        status.running = false;
        status.finished = true;
        status.state = "Error";
        status.error = "WiFi not connected";

        websocket.notifyOTA(status);

        return false;
    }

    String url =
        versionURL + "?ts=" + String(esp_random());

    HTTPClient http;

    http.setReuse(false);
    http.useHTTP10(true);
    http.setFollowRedirects(
        HTTPC_FORCE_FOLLOW_REDIRECTS
    );

    http.addHeader("Cache-Control", "no-cache");
    http.addHeader("Pragma", "no-cache");

    http.begin(url);

    int code = http.GET();

    if (code != HTTP_CODE_OK)
    {
        status.running = false;
        status.finished = true;
        status.state = "Error";
        status.error = "HTTP " + String(code);

        websocket.notifyOTA(status);

        http.end();

        return false;
    }

    String payload = http.getString();

    http.end();

    JsonDocument doc;

    DeserializationError err =
        deserializeJson(doc, payload);

    if (err)
    {
        status.running = false;
        status.finished = true;
        status.state = "Error";
        status.error = err.c_str();

        websocket.notifyOTA(status);

        return false;
    }

    remoteVersion =
        doc["version"].as<String>();

    firmwareURL =
        doc["url"].as<String>();

    bool available =
        isNewVersion(remoteVersion, currentVersion);

    status.running = false;
    status.finished = true;
    status.success = available;

    status.state =
        available
        ? "Update Available"
        : "Up To Date";

    websocket.notifyOTA(status);

    return available;
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

    websocket.notifyOTA(status);

    HTTPClient http;

    http.setFollowRedirects(
        HTTPC_FORCE_FOLLOW_REDIRECTS
    );

    http.begin(url);

    int code = http.GET();

    if(code != HTTP_CODE_OK)
    {
        status.running = false;
        status.finished = true;
        status.success = false;

        status.state = "Error";
        status.error = "HTTP " + String(code);

        websocket.notifyOTA(status);

        http.end();

        return false;
    }

    status.total = http.getSize();

    if(!Update.begin(status.total))
    {
        status.running = false;
        status.finished = true;
        status.success = false;

        status.state = "Error";
        status.error = "Not enough flash";

        websocket.notifyOTA(status);

        http.end();

        return false;
    }

    WiFiClient* stream = http.getStreamPtr();

    uint8_t buffer[1024];

    uint32_t lastMillis = millis();
    uint32_t lastBytes = 0;

    uint8_t lastPercent = 255;

    status.state = "Downloading";

    websocket.notifyOTA(status);

    while(http.connected() &&
          status.downloaded < status.total)
    {
        size_t available =
            stream->available();

        if(available)
        {
            int len =
                stream->readBytes(
                    buffer,
                    min((size_t)1024, available)
                );

            if(len > 0)
            {
                size_t written =
                    Update.write(buffer, len);

                if(written != (size_t)len)
                {
                    status.running = false;
                    status.finished = true;
                    status.success = false;

                    status.state = "Error";
                    status.error =
                        Update.errorString();

                    websocket.notifyOTA(status);

                    http.end();

                    return false;
                }

                status.downloaded += len;

                status.percent =
                    (status.downloaded * 100)
                    / status.total;

                if(status.percent != lastPercent)
                {
                    lastPercent = status.percent;

                    websocket.notifyOTA(status);
                }
            }
        }

        if(millis() - lastMillis >= 1000)
        {
            uint32_t diff =
                status.downloaded - lastBytes;

            status.speedKB =
                diff / 1024.0f;

            if(status.speedKB > 0)
            {
                status.eta =
                    ((status.total -
                      status.downloaded)
                     / 1024.0f)
                    / status.speedKB;
            }
            else
            {
                status.eta = 0;
            }

            websocket.notifyOTA(status);

            lastBytes = status.downloaded;
            lastMillis = millis();
        }

        delay(1);
    }

    status.state = "Verifying";

    websocket.notifyOTA(status);

    if(!Update.end())
    {
        status.running = false;
        status.finished = true;
        status.success = false;

        status.state = "Error";
        status.error =
            Update.errorString();

        websocket.notifyOTA(status);

        http.end();

        return false;
    }

    if(!Update.isFinished())
    {
        status.running = false;
        status.finished = true;
        status.success = false;

        status.state = "Error";
        status.error =
            "Update not finished";

        websocket.notifyOTA(status);

        http.end();

        return false;
    }

    http.end();

    status.running = false;
    status.finished = true;
    status.success = true;

    status.percent = 100;
    status.speedKB = 0;
    status.eta = 0;

    status.state = "Completed";

    websocket.notifyOTA(status);

    return true;
}
bool OTAService::updateFirmware()
{
    if(firmwareURL.isEmpty())
    {
        status.running = false;
        status.finished = true;
        status.success = false;

        status.state = "Error";
        status.error = "Firmware URL is empty";

        websocket.notifyOTA(status);

        return false;
    }

    return downloadAndUpdate(firmwareURL);
}

void OTAService::checkTask(void* parameter)
{
    OTAService* self =
        static_cast<OTAService*>(parameter);

    self->checkForUpdate();

    vTaskDelete(nullptr);
}

void OTAService::updateTask(void* parameter)
{
    OTAService* self =
        static_cast<OTAService*>(parameter);

    self->updateFirmware();

    if(self->status.success)
    {
        delay(1000);

        ESP.restart();
    }

    vTaskDelete(nullptr);
}
