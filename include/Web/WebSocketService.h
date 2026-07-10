#ifndef WEB_SOCKET_SERVICE_H
#define WEB_SOCKET_SERVICE_H
#include "Web/Message.h"
#include <ESPAsyncWebServer.h>
#include "OTA/OTAStatus.h"

class WebSocketService
{
public:
    void begin(AsyncWebServer& server);
    void broadcast(const String& json);
    void send(const Message& message);

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
