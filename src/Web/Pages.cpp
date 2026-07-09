#include "Web/Pages.h"
#include "Web/WebServerService.h"
#include "WebUI/index_html.h"
#include "WebUI/style_css.h"
#include "WebUI/app_js.h"
#include "WebUI/ota_html.h"
#include <LittleFS.h>

namespace Pages
{

    void registerRoutes(WebServerService& web)
    {
        auto& server = web.server();
    
        if(!LittleFS.begin(true))
        {
            Serial.println("LittleFS Mount Failed");
            return;
        }
    
        server.on("/", HTTP_GET,
        [](AsyncWebServerRequest *request)
        {
            request->send_P(
                200,
                "text/html",
                INDEX_HTML
            );
        });
        
        server.on("/style.css", HTTP_GET,
        [](AsyncWebServerRequest *request)
        {
            request->send_P(
                200,
                "text/css",
                STYLE_CSS
            );
        });
        
        server.on("/app.js", HTTP_GET,
        [](AsyncWebServerRequest *request)
        {
            request->send_P(
                200,
                "application/javascript",
                APP_JS
            );
        });
        
        server.on("/ota", HTTP_GET,
        [](AsyncWebServerRequest *request)
        {
            request->send_P(
                200,
                "text/html",
                OTA_HTML
            );
        });
    }

}
