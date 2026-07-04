#pragma once

#include <WebServer.h>
#include "github.h"
#include "ota.h"

class Web
{
public:
    Web();

    void begin();
    void handle();

private:
    WebServer server;

    GitHub github;
    OTA ota;

    void setupRoutes();

    void root();

    void apiInfo();

    void apiCheck();

    void apiUpdate();

    void apiProgress();
};
