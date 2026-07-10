#include "Web/WebSocketService.h"
#include <ArduinoJson.h>
#include "OTA/OTAService.h"

extern OTAService ota;
WebSocketService websocket;
extern WebSocketService websocket;

void WebSocketService::notifyOTA(const OTAStatus& st)
{
    JsonDocument doc;

    doc["type"] = "ota";

    doc["running"] = st.running;
    doc["finished"] = st.finished;
    doc["success"] = st.success;

    doc["downloaded"] = st.downloaded;
    doc["total"] = st.total;

    doc["percent"] = st.percent;

    doc["speed"] = st.speedKB;

    doc["eta"] = st.eta;

    doc["state"] = st.state;

    doc["error"] = st.error;

    String json;

    serializeJson(doc, json);

    ws.textAll(json);
}
void WebSocketService::notifyReload()
{
    JsonDocument doc;

    doc["type"] = "reload";

    String json;
    serializeJson(doc, json);

    ws.textAll(json);
}

void WebSocketService::notifyChannel(uint16_t id, bool state)
{
    JsonDocument doc;

    doc["type"] = "channel";
    doc["id"] = id;
    doc["state"] = state;

    String json;
    serializeJson(doc, json);

    ws.textAll(json);
}

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
        
            client->text(R"({
                "type":"hello",
                "message":"Connected"
            })");
            
            break;

        case WS_EVT_DISCONNECT:
            Serial.printf("WS Client %u Disconnected\n", client->id());
            break;

        case WS_EVT_DATA:
        {
            AwsFrameInfo* info = (AwsFrameInfo*)arg;
        
            if (!info->final)
                break;
        
            if (info->index != 0)
                break;
        
            if (info->opcode != WS_TEXT)
                break;
        
            String json;
        
            json.reserve(len);
        
            for (size_t i = 0; i < len; i++)
            {
                json += (char)data[i];
            }
        
            JsonDocument doc;
        
            DeserializationError err =
                deserializeJson(doc, json);
        
            if (err)
            {
                Serial.println("WS Invalid JSON");
                break;
            }
        
            String cmd = doc["cmd"] | "";
        
            Serial.print("WS Command: ");
            Serial.println(cmd);
        
            if (cmd == "ota.check")
            {
                ota.startCheck();
            }
            else if (cmd == "ota.update")
            {
                ota.startUpdate();
            }
        
            break;
        }
        case WS_EVT_PONG:
            break;

        case WS_EVT_ERROR:
            break;
    }
}
