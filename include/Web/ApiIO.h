#ifndef API_IO_H
#define API_IO_H

class WebServerService;

namespace ApiIO
{
    void registerRoutes(WebServerService& web);

    void handleToggle(WebServerService& web);
}
#endif
