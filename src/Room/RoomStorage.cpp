#include "Room/RoomStorage.h"

#include <LittleFS.h>
#include <ArduinoJson.h>


bool RoomStorage::save(RoomRepository& repository)
{
    File f = LittleFS.open("/rooms.json", "w");

    if (!f)
        return false;


    JsonDocument doc;

    JsonArray arr = doc.to<JsonArray>();


    for (uint8_t i = 0; i < repository.count(); i++)
    {
        Room* room = repository.get(i);

        JsonObject o = arr.add<JsonObject>();

        o["id"] = room->id;
        o["name"] = room->name;
        o["icon"] = room->icon;
        o["enabled"] = room->enabled;
        o["favorite"] = room->favorite;
    }


    serializeJson(doc, f);

    f.close();

    return true;
}



bool RoomStorage::load(RoomRepository& repository)
{
    if (!LittleFS.exists("/rooms.json"))
        return false;


    File f = LittleFS.open("/rooms.json", "r");

    if (!f)
        return false;


    JsonDocument doc;


    if (deserializeJson(doc, f))
    {
        f.close();
        return false;
    }


    JsonArray arr = doc.as<JsonArray>();


    for (JsonObject o : arr)
    {
        Room room;

        room.id = o["id"] | 0;
        room.name = o["name"] | "Room";
        room.icon = o["icon"] | "home";
        room.enabled = o["enabled"] | true;
        room.favorite = o["favorite"] | false;


        repository.add(room);
    }


    f.close();

    return true;
}
