#ifndef WEB_SOCKET_SERVICE_H
#define WEB_SOCKET_SERVICE_H

#include <ESPAsyncWebServer.h>

class WebSocketService
{
public:
    void begin(AsyncWebServer& server);
    void broadcast(const String& json);

private:
    AsyncWebSocket ws{"/ws"};

    static void onEvent(
        AsyncWebSocket* server,
        AsyncWebSocketClient* client,
        AwsEventType type,
        void* arg,
        uint8_t* data,
        size_t len
    );
};

extern WebSocketService websocket;

#endif
