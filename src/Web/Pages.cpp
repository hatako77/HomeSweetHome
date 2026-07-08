#include "Web/Pages.h"
#include "Web/WebServerService.h"
#include <LittleFS.h>
#include "WebUI/ota_html.h"


namespace Pages
{

    void registerRoutes(WebServerService& web)
    {
        auto& server = web.server();
    
        if (!LittleFS.begin(true))
        {
            Serial.println("LittleFS Mount Failed");
        }
    
        server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");
    
        server.serveStatic("/style.css", LittleFS, "/style.css");
        server.serveStatic("/app.js", LittleFS, "/app.js");
    
        server.serveStatic("/icons/", LittleFS, "/icons/");
    
        server.on("/ota", HTTP_GET, [&server]()
        {
            server.send_P(200, "text/html", OTA_HTML);
        });
    }

}
