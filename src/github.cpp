#include "github.h"

#include "config.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

GitHub::GitHub()
{
    _latestVersion="";
    _lastError="";
}

bool GitHub::begin()
{
    return WiFi.status()==WL_CONNECTED;
}

bool GitHub::downloadVersionFile(String &json)
{
    HTTPClient http;

    String url=versionURL();

    url+="?t=";

    url+=String(millis());

    http.begin(url);

    http.setFollowRedirects(
        HTTPC_STRICT_FOLLOW_REDIRECTS);

    http.setTimeout(HTTP_TIMEOUT);

    http.addHeader(
        "Cache-Control",
        "no-cache");

    int code=http.GET();

    if(code!=HTTP_CODE_OK)
    {
        _lastError="HTTP "+String(code);

        http.end();

        return false;
    }

    json=http.getString();

    http.end();

    return true;
}

bool GitHub::check()
{
    if(WiFi.status()!=WL_CONNECTED)
    {
        _lastError="WiFi disconnected";

        return false;
    }

    String json;

    if(!downloadVersionFile(json))
        return false;

    JsonDocument doc;

    DeserializationError err=
        deserializeJson(doc,json);

    if(err)
    {
        _lastError="JSON parse failed";

        return false;
    }

    if(!doc["version"].is<String>())
    {
        _lastError="Invalid version.json";

        return false;
    }

    _latestVersion=
        doc["version"].as<String>();

    return true;
}

String GitHub::latestVersion() const
{
    return _latestVersion;
}

String GitHub::versionURL() const
{
    return
    "https://raw.githubusercontent.com/"
    +String(GITHUB_OWNER)
    +"/"
    +String(GITHUB_REPOSITORY)
    +"/main/version.json";
}

String GitHub::firmwareURL() const
{
    return
    "https://github.com/"
    +String(GITHUB_OWNER)
    +"/"
    +String(GITHUB_REPOSITORY)
    +"/releases/download/v"
    +_latestVersion
    +"/firmware.bin";
}

String GitHub::changelogURL() const
{
    return
    "https://raw.githubusercontent.com/"
    +String(GITHUB_OWNER)
    +"/"
    +String(GITHUB_REPOSITORY)
    +"/main/CHANGELOG.md";
}

String GitHub::lastError() const
{
    return _lastError;
}
