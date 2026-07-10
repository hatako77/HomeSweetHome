#include "Web/WebServerService.h"
#include "Web/Pages.h"
#include "Web/ApiVersion.h"
#include "Web/ApiIO.h"
#include "Web/ApiOTA.h"
#include "Web/ApiSettings.h"
#include "OTA/OTAService.h"
#include "Web/ApiRoom.h"
#include "Web/ApiChannel.h"

WebServerService web;
extern OTAService ota;

void WebServerService::begin()
{
    Serial.println("1");

    registerPages();

    Serial.println("2");

    registerApi();

    Serial.println("3");

    webServer.begin();

    Serial.println("4");
}

void WebServerService::update()
{
}

AsyncWebServer& WebServerService::server()
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
    ApiRoom::registerRoutes(*this);
    ApiChannel::registerRoutes(*this);
}
