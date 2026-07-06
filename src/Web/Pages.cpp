#include "Web/Pages.h"
#include "Web/WebServerService.h"

#include "WebUI/index_html.h"
#include "WebUI/ota_html.h"

namespace Pages
{

void registerRoutes(WebServerService& web)
{
    auto& server = web.server();

    server.on("/", HTTP_GET, [&]()
    {
        server.send_P(200, "text/html", INDEX_HTML);
    });

    server.on("/ota", HTTP_GET, [&]()
    {
        server.send_P(200, "text/html", OTA_HTML);
    });

    // بعداً
    // server.on("/settings", HTTP_GET, ...);
}

}
