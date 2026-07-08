#include "Web/Pages.h"
#include "Web/WebServerService.h"
#include <LittleFS.h>
#include "WebUI/ota_html.h"


namespace Pages
{

void registerRoutes(WebServerService& web)
{
    auto& server = web.server();
    server.on("/", HTTP_GET, [&server]()
    {
        server.send_P(200, "text/html", INDEX_HTML);
    });
    server.on("/ota", HTTP_GET, [&server]()
    {
        server.send_P(200, "text/html", OTA_HTML);
    });
    server.on("/style.css", HTTP_GET, [&server]()
    {
        server.send_P(200, "text/css", STYLE_CSS);
    });
    server.on("/app.js", HTTP_GET, [&server]()
    {
        server.send_P(200, "application/javascript", APP_JS);
    });

    // بعداً:
    // server.on("/settings", HTTP_GET, ...);
}

}
