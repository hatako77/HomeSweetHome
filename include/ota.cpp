#include "ota.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <Update.h>

OTA::OTA()
{
    _progress = 0;
    _running = false;
    _status = "Idle";
}

struct OTAContext
{
    OTA *ota;
    String url;
};

bool OTA::begin(const String &url)
{
    if (_running)
        return false;

    OTAContext *ctx = new OTAContext;
    ctx->ota = this;
    ctx->url = url;

    _running = true;

    xTaskCreatePinnedToCore(
        task,
        "OTA",
        8192,
        ctx,
        1,
        nullptr,
        1);

    return true;
}

void OTA::task(void *parameter)
{
    OTAContext *ctx = (OTAContext *)parameter;

    ctx->ota->updateTask(ctx->url);

    delete ctx;

    vTaskDelete(nullptr);
}

bool OTA::updateTask(const String &url)
{
    _progress = 0;
    _status = "Connecting...";

    HTTPClient http;

    http.begin(url);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.setTimeout(30000);

    int code = http.GET();

    if (code != HTTP_CODE_OK)
    {
        _status = "HTTP Error";
        _running = false;
        http.end();
        return false;
    }

    int size = http.getSize();

    if (size <= 0)
    {
        _status = "Invalid File";
        _running = false;
        http.end();
        return false;
    }

    if (!Update.begin(size))
    {
        _status = "Update Begin Failed";
        _running = false;
        http.end();
        return false;
    }

    WiFiClient *stream = http.getStreamPtr();

    uint8_t buffer[4096];

    size_t written = 0;

    while (http.connected() && written < (size_t)size)
    {
        size_t available = stream->available();

        if (available == 0)
        {
            vTaskDelay(1);
            continue;
        }

        size_t len = available;

        if (len > sizeof(buffer))
            len = sizeof(buffer);

        int read = stream->readBytes(buffer, len);

        if (read <= 0)
            continue;

        if (Update.write(buffer, read) != (size_t)read)
        {
            Update.abort();

            _status = "Write Error";
            _running = false;

            http.end();

            return false;
        }

        written += read;

        _progress = (written * 100) / size;

        _status =
            "Updating " +
            String(_progress) +
            "%";

        vTaskDelay(1);
    }

    http.end();

    if (!Update.end(true))
    {
        _status = "Finish Error";
        _running = false;
        return false;
    }

    _progress = 100;
    _status = "Done";
    _running = false;

    delay(1000);

    ESP.restart();

    return true;
}

int OTA::progress()
{
    return _progress;
}

String OTA::status()
{
    return _status;
}

bool OTA::running()
{
    return _running;
}
