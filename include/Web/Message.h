#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>

class Message
{
public:

    String type;
    String action;

    JsonDocument data;

    Message(
        const String& t,
        const String& a)
        : type(t),
          action(a)
    {
    }

    String serialize() const
    {
        JsonDocument doc;

        doc["type"] = type;
        doc["action"] = action;

        doc["data"] = data.as<JsonVariantConst>();

        String json;

        serializeJson(doc, json);

        return json;
    }
};
