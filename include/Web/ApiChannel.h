#ifndef API_CHANNEL_H
#define API_CHANNEL_H

class WebServerService;

class ApiChannel
{
public:
    static void registerRoutes(WebServerService& web);
};

#endif
