#include "Web/WebSocketService.h"

#include <ArduinoJson.h>

#include "OTA/OTAService.h"
#include "Web/Message.h"

extern OTAService ota;

WebSocketService websocket;

void WebSocketService::send(const Message& message)
{
    ws.textAll(message.serialize());
}

void WebSocketService::broadcast(const String& json)
{
    ws.textAll(json);
}

void WebSocketService::begin(AsyncWebServer& server)
{
    ws.onEvent(onEvent);
    server.addHandler(&ws);
}

void WebSocketService::onEvent(
    AsyncWebSocket* server,
    AsyncWebSocketClient* client,
    AwsEventType eventType,
    void* arg,
    uint8_t* data,
    size_t len)
{
    switch(eventType)
    {
        case WS_EVT_CONNECT:
        {
            Serial.printf("WS Client %u Connected\n", client->id());

            Message msg("system","connected");

            client->text(msg.serialize());

            break;
        }

        case WS_EVT_DISCONNECT:
        {
            Serial.printf("WS Client %u Disconnected\n", client->id());
            break;
        }

        case WS_EVT_DATA:
        {
            AwsFrameInfo* info=(AwsFrameInfo*)arg;

            if(!info->final)
                break;

            if(info->index!=0)
                break;

            if(info->opcode!=WS_TEXT)
                break;

            String json;

            json.reserve(len);

            for(size_t i=0;i<len;i++)
                json+=(char)data[i];

            JsonDocument doc;

            if(deserializeJson(doc,json))
            {
                Serial.println("WS Invalid JSON");
                break;
            }

            String type   = doc["type"]   | "";
            String action = doc["action"] | "";

            JsonObject payload = doc["data"].as<JsonObject>();

            Serial.print("WS ");
            Serial.print(type);
            Serial.print(" -> ");
            Serial.println(action);

            //--------------------------------------------------
            // OTA
            //--------------------------------------------------

            if(type=="ota")
            {
                if(action=="check")
                {
                    ota.startCheck();
                }
                else if(action=="update")
                {
                    ota.startUpdate();
                }
            }

            //--------------------------------------------------
            // CHANNEL
            //--------------------------------------------------

            else if(type=="channel")
            {
                // next step
            }

            //--------------------------------------------------
            // ROOM
            //--------------------------------------------------

            else if(type=="room")
            {
                // next step
            }

            //--------------------------------------------------
            // SCENE
            //--------------------------------------------------

            else if(type=="scene")
            {
                // next step
            }

            //--------------------------------------------------
            // SENSOR
            //--------------------------------------------------

            else if(type=="sensor")
            {
                // next step
            }

            //--------------------------------------------------
            // SETTINGS
            //--------------------------------------------------

            else if(type=="settings")
            {
                // next step
            }

            break;
        }

        case WS_EVT_PONG:
            break;

        case WS_EVT_ERROR:
            break;
    }
}
