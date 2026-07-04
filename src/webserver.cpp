//======================== src/webserver.cpp ========================

#include "webserver.h"
#include "ota.h"
#include "version.h"

#include <ESPAsyncWebServer.h>
#include <LittleFS.h>

AsyncWebServer server(80);

void webserverInit()
{

    server.serveStatic("/",LittleFS,"/").setDefaultFile("index.html");

    server.serveStatic("/css",LittleFS,"/css");

    server.serveStatic("/js",LittleFS,"/js");

    server.on("/ota",HTTP_GET,[](AsyncWebServerRequest *request)
    {
        request->send(LittleFS,"/ota.html","text/html");
    });

    server.on("/api/version",HTTP_GET,[](AsyncWebServerRequest *request)
    {
        request->send(200,"application/json",
        String("{\"version\":\"")+getVersion()+"\"}");
    });

    server.on("/api/check",HTTP_GET,[](AsyncWebServerRequest *request)
    {
        request->send(200,"application/json",
        checkGithubVersion());
    });

    registerOTA(server);

    server.begin();
}
