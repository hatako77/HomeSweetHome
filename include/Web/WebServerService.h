#ifndef WEB_SERVER_SERVICE_H
#define WEB_SERVER_SERVICE_H

#include <WebServer.h>

class WebServerService
{
public:
    void begin();
    void update();

    WebServer& server();

private:
    WebServer webServer{80};

    void registerPages();
    void registerApi();
};

extern WebServerService web;

#endif
