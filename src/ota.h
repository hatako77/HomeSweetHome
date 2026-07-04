//======================== src/ota.h ========================

#pragma once

#include <ESPAsyncWebServer.h>

void otaInit();

void registerOTA(AsyncWebServer& server);

String checkGithubVersion();

bool autoOTA();
