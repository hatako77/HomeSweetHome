#include "Web/WebSocketService.h"

WebSocketService websocket;

void WebSocketService::begin(AsyncWebServer& server)
{
    ws.onEvent(onEvent);

    server.addHandler(&ws);
}

void WebSocketService::broadcast(const String& json)
{
    ws.textAll(json);
}

void WebSocketService::onEvent(
    AsyncWebSocket* server,
    AsyncWebSocketClient* client,
    AwsEventType type,
    void* arg,
    uint8_t* data,
    size_t len)
{
    switch(type)
    {
        case WS_EVT_CONNECT:
            Serial.printf("WS Client %u Connected\n", client->id());
            break;

        case WS_EVT_DISCONNECT:
            Serial.printf("WS Client %u Disconnected\n", client->id());
            break;

        case WS_EVT_DATA:
            // بعداً استفاده می‌کنیم
            break;

        case WS_EVT_PONG:
            break;

        case WS_EVT_ERROR:
            break;
    }
}
