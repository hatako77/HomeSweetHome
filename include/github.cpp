#include "github.h"
#include "config.h"

#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

GitHub::GitHub()
{
    _latestVersion = "";
}

bool GitHub::checkVersion()
{
    if (WiFi.status() != WL_CONNECTED)
        return false;

    HTTPClient http;

    String url =
        "https://raw.githubusercontent.com/" +
        String(GITHUB_USER) + "/" +
        String(GITHUB_REPO) +
        "/main/" +
        String(VERSION_FILE);

    http.begin(url);
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.setTimeout(5000);

    int code = http.GET();

    if (code != HTTP_CODE_OK)
    {
        http.end();
        return false;
    }

    DynamicJsonDocument doc(512);

    DeserializationError err =
        deserializeJson(doc, http.getString());

    http.end();

    if (err)
        return false;

    _latestVersion = doc["version"].as<String>();

    return true;
}

String GitHub::latestVersion()
{
    return _latestVersion;
}

String GitHub::firmwareURL()
{
    return
        "https://github.com/" +
        String(GITHUB_USER) +
        "/" +
        String(GITHUB_REPO) +
        "/releases/download/v" +
        _latestVersion +
        "/" +
        String(FIRMWARE_FILE);
}
