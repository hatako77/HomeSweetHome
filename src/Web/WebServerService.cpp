#include "Web/WebServerService.h"

WebServerService web;

void WebServerService::begin()
{
    registerPages();
    registerApi();

    webServer.begin();

    Serial.println("WebServer started");
}

void WebServerService::update()
{
    webServer.handleClient();
}

WebServer& WebServerService::server()
{
    return webServer;
}

void WebServerService::registerPages()
{
    // TODO
}

void WebServerService::registerApi()
{
    // TODO
}
