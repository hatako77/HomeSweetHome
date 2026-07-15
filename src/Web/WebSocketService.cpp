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

void WebSocketService::loop()
{
    ws.cleanupClients();
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
        {
            Serial.printf(
                "WS Client %u Connected\n",
                client->id()
            );

            Message msg("system","connected");

            client->text(msg.serialize());

            break;
        }

        case WS_EVT_DISCONNECT:
        {
            Serial.printf(
                "WS Client %u Disconnected\n",
                client->id()
            );

            break;
        }

        case WS_EVT_PONG:
        {
            break;
        }

        case WS_EVT_ERROR:
        {
            Serial.println("WS Error");
            break;
        }

        case WS_EVT_DATA:
        {
            AwsFrameInfo* info =(AwsFrameInfo*)arg;
            if(!info->final)return;
            if(info->index != 0)return;
            if(info->opcode != WS_TEXT)return;            
            String json;
            json.reserve(len);            
            for(size_t i=0;i<len;i++)json += (char)data[i];            
            Serial.println("========== WS ==========");
            Serial.println(json);
            Serial.println("========================");

            JsonDocument doc;
            if(deserializeJson(doc,json))
            {
                Serial.println("WS Invalid JSON");
                return;
            }
            String type =doc["type"] | "";
            String action =doc["action"] | "";
            JsonObject data =doc["data"].as<JsonObject>();
            Serial.print("TYPE   : ");
            Serial.println(type);            
            Serial.print("ACTION : ");
            Serial.println(action);

            //--------------------------------------------------
            // OTA
            //--------------------------------------------------

            if(type == "ota")
            {
                if(action == "check")
                {
                    ota.startCheck();
                }
                else if(action == "update")
                {
                    ota.startUpdate();
                }
                return;
            }

            //--------------------------------------------------
            // Future Commands
            //--------------------------------------------------

            if(type == "channel")
            {
                uint16_t id = data["id"] | 0;
                if(action == "toggle")
                {
                    if(ioManager.toggle(id))
                    {
                        ioManager.save();            
                        IOChannel* ch = ioManager.getChannel(id);            
                        if(ch) Notifier::channelChanged(*ch);
                    }
                }
                else if(action == "on")
                {
                    if(ioManager.on(id))
                    {
                        ioManager.save();            
                        IOChannel* ch = ioManager.getChannel(id);            
                        if(ch) Notifier::channelChanged(*ch);
                    }
                }
                else if(action == "off")
                {
                    if(ioManager.off(id))
                    {
                        ioManager.save();            
                        IOChannel* ch = ioManager.getChannel(id);            
                        if(ch) Notifier::channelChanged(*ch);
                    }
                }            
                return;
            }

            if(type == "room")
            {
                return;
            }

            if(type == "scene")
            {
                return;
            }

            if(type == "schedule")
            {
                return;
            }

            if(type == "sensor")
            {
                return;
            }

            Serial.printf(
                "Unknown WS Message : %s / %s\n",
                type.c_str(),
                action.c_str()
            );

            break;
        }
    }
}
