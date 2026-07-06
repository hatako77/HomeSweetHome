#include "IO/IOStorage.h"

#include <LittleFS.h>
#include <ArduinoJson.h>

#include "IO/IOManager.h"

bool IOStorage::save()
{
    File f = LittleFS.open("/io.json", "w");

    if (!f)
        return false;

    JsonDocument doc;

    JsonArray arr = doc.to<JsonArray>();

    for (uint8_t i = 0; i < ioManager.count(); i++)
    {
        auto* ch = ioManager.getChannel(i);

        JsonObject o = arr.add<JsonObject>();

        o["name"] = ch->name;
        o["enabled"] = ch->enabled;
        o["activeLow"] = ch->activeLow;
        o["type"] = (uint8_t)ch->type;
        o["icon"] = (uint8_t)ch->icon;
        o["favorite"] = ch->favorite;
        o["roomId"] = ch->roomId;
        o["groupId"] = ch->groupId;
    }

    serializeJson(doc, f);

    f.close();

    return true;
}

bool IOStorage::load()
{
    if (!LittleFS.exists("/io.json"))
        return false;

    File f = LittleFS.open("/io.json", "r");

    if (!f)
        return false;

    JsonDocument doc;

    deserializeJson(doc, f);

    JsonArray arr = doc.as<JsonArray>();

    uint8_t i = 0;

    for (JsonObject o : arr)
    {
        if (i >= ioManager.count())
            break;

        auto* ch = ioManager.get(i);

        ch->name = o["name"] | ch->name;
        ch->enabled = o["enabled"] | true;
        ch->activeLow = o["activeLow"] | false;
        IOType type;
        if (typeFromValue(o["type"] | 1, type))
            ch->type = type;
        if (typeFromValue(o["icon"] | 1, icon))
            ch->icon = icon;
        ch->favorite = o["favorite"] | false;
        ch->roomId = o["roomId"] | 0;
        ch->groupId = o["groupId"] | 0;
        i++;
    }

    f.close();

    return true;
}
