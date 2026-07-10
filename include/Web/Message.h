#ifndef MESSAGE_H
#define MESSAGE_H

#include <ArduinoJson.h>

class Message
{
public:

    JsonDocument json;

    Message(const String& type,
            const String& action)
    {
        json["type"] = type;
        json["action"] = action;
    }

    String serialize() const
    {
        String s;
        serializeJson(json, s);
        return s;
    }
};

#endif
