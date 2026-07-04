//======================== src/ota.cpp ========================

#include "ota.h"

#include <HTTPClient.h>
#include <HTTPUpdate.h>
#include <ArduinoJson.h>

#include "version.h"

String latestFirmwareUrl;

void otaInit()
{

}

String checkGithubVersion()
{

    HTTPClient http;

    http.begin(getVersionURL());

    int code=http.GET();

    if(code!=200)
    {
        return "{\"update\":false}";
    }

    DynamicJsonDocument doc(1024);

    deserializeJson(doc,http.getString());

    String latest=doc["version"];

    latestFirmwareUrl=doc["firmware"];

    if(latest==getVersion())
    {
        return "{\"update\":false}";
    }

    return String("{\"update\":true,\"latest\":\"")+latest+"\"}";
}

bool autoOTA()
{

    WiFiClient client;

    t_httpUpdate_return ret=httpUpdate.update(client,latestFirmwareUrl);

    switch(ret)
    {
        case HTTP_UPDATE_FAILED:

            return false;

        case HTTP_UPDATE_NO_UPDATES:

            return false;

        case HTTP_UPDATE_OK:

            return true;
    }

    return false;

}

void registerOTA(AsyncWebServer &server)
{

server.on("/api/update",HTTP_POST,

[](AsyncWebServerRequest *request)
{

request->send(200,"text/plain","OK");

ESP.restart();

},

[](AsyncWebServerRequest *request,
String filename,
size_t index,
uint8_t *data,
size_t len,
bool final)
{

if(!index)
{

Update.begin(UPDATE_SIZE_UNKNOWN);

}

Update.write(data,len);

if(final)
{

Update.end(true);

}

});

}
