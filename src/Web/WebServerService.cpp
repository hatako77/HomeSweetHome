#include "Web/WebServerService.h"
#include "Web/Pages.h"
#include "Web/ApiVersion.h"
#include "Web/ApiIO.h"
#include "Web/ApiOTA.h"
#include "Web/ApiSettings.h"
#include "OTA/OTAService.h"
#include "Web/ApiRoom.h"
#include "Web/ApiChannel.h"
#include "Web/WebSocketService.h"
#include "Web/ApiSystem.h"

WebServerService web;
extern OTAService ota;

void WebServerService::begin()
{
    registerPages();
    registerApi();
    websocket.begin(webServer);
    webServer.begin();
    Serial.println("WebServer started");
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
    ApiSystem::registerRoutes(*this);
    ApiScene::registerRoutes(*this);

}
