#include "IO/IOStorage.h"
#include "IO/TypeHelper.h"
#include "IO/IconHelper.h"
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
        o["id"] = ch->id;        
        o["name"] = ch->name;        
        o["enabled"] = ch->enabled;
        o["favorite"] = ch->favorite;        
        o["activeLow"] = ch->activeLow;        
        o["type"] = static_cast<uint8_t>(ch->type);
        o["icon"] = static_cast<uint8_t>(ch->icon);        
        o["roomId"] = ch->roomId;
        o["groupId"] = ch->groupId;        
        o["driverId"] = ch->address.driverId;
        o["device"] = ch->address.device;
        o["pin"] = ch->address.pin;
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

        auto* ch = ioManager.getChannel(i);

        ch->name = o["name"] | ch->name;
        ch->enabled = o["enabled"] | true;
        ch->activeLow = o["activeLow"] | false;
        IOType type;
        if (typeFromValue(o["type"] | 1, type))
            ch->type = type;
        IOIcon icon;        
        if (iconFromValue(o["icon"] | 0, icon))
            ch->icon = icon;
        ch->favorite = o["favorite"] | false;
        ch->roomId = o["roomId"] | 0;
        ch->groupId = o["groupId"] | 0;
        ch->favorite = o["favorite"] | false;
        ch->address.driverId = o["driverId"] | 0;
        ch->address.device = o["device"] | 0;
        ch->address.pin = o["pin"] | 0;
        i++;
    }

    f.close();

    return true;
}
