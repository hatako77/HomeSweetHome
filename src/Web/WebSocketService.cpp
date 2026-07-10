#include "Web/WebSocketService.h"
#include <ArduinoJson.h>
#include "OTA/OTAService.h"
#include "Web/Message.h"

extern OTAService ota;
WebSocketService websocket;
extern WebSocketService websocket;

void WebSocketService::notifyOTA(const OTAStatus& st)
{
    extern OTAService ota;
    Message msg("ota", "progress");
    msg.json["running"] = st.running;
    msg.json["finished"] = st.finished;
    msg.json["success"] = st.success;
    msg.json["downloaded"] = st.downloaded;
    msg.json["total"] = st.total;
    msg.json["percent"] = st.percent;
    msg.json["speed"] = st.speedKB;
    msg.json["eta"] = st.eta;
    msg.json["state"] = st.state;
    msg.json["error"] = st.error;
    msg.json["current"] = ota.getCurrentVersion();
    msg.json["remote"]  = ota.getRemoteVersion();
    // بعداً از این برای تشخیص Check و Update استفاده می‌کنیم
    msg.json["update"] =
        ota.getRemoteVersion() != ota.getCurrentVersion();
    ws.textAll(msg.serialize());
}


void WebSocketService::notifyReload()
{
    Message msg("room", "reload");

    ws.textAll(msg.serialize());
}

void WebSocketService::notifyChannel(uint16_t id, bool state)
{
    Message msg("channel", "state");

    msg.json["id"] = id;
    msg.json["state"] = state;

    ws.textAll(msg.serialize());
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
