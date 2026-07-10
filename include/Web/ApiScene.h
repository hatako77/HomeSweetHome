#pragma once

#include <ESPAsyncWebServer.h>

class ApiScene
{
public:
    static void registerRoutes(AsyncWebServer& server);
};
