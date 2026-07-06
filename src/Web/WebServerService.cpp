#include "Web/WebServerService.h"
#include "Web/Pages.h"
#include "Web/ApiVersion.h"
#include "Web/ApiIO.h"
#include "Web/ApiOTA.h"
#include "Web/ApiSettings.h"


WebServerService web;
extern OTAService ota;

void WebServerService::begin()
{
    registerPages();
    registerApi();

    webServer.begin();
     ota.registerRoutes(webServer); 
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
    ApiVersion::registerRoutes(*this);
    ApiIO::registerRoutes(*this);
    ApiOTA::registerRoutes(*this);
    ApiSettings::registerRoutes(*this);
}
