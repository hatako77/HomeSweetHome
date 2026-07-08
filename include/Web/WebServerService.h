#ifndef WEB_SERVER_SERVICE_H
#define WEB_SERVER_SERVICE_H

#include <ESPAsyncWebServer.h>

class WebServerService
{
public:
    void begin();
    void update();

    AsyncWebServer& server();

private:
    AsyncWebServer webServer{80};

    void registerPages();
    void registerApi();
};

extern WebServerService web;

#endif
