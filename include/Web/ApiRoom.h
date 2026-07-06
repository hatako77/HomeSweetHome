#ifndef API_ROOM_H
#define API_ROOM_H

#include "Web/WebServerService.h"

class ApiRoom
{
public:
    static void registerRoutes(WebServerService& web);
};

#endif
