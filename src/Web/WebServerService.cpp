#include "Web/WebServerService.h"
#include "Web/Pages.h"

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
    Pages::registerRoutes(*this);
}

void WebServerService::registerApi()
{
    // فعلاً خالی
}
