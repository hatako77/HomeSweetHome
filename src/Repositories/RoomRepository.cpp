#include "Repositories/RoomRepository.h"

#include <ArduinoJson.h>
#include <FS.h>

#include "Core/FileStorage.h"
#include "Core/Paths.h"

RoomRepository roomRepository;

void RoomRepository::begin()
{
    load();
}

uint8_t RoomRepository::count() const
{
    return roomCount;
}

Room* RoomRepository::get(uint16_t id)
{
    for (uint8_t i = 0; i < roomCount; i++)
    {
        if (rooms[i].id == id)
            return &rooms[i];
    }

    return nullptr;
}

bool RoomRepository::add(const Room& room)
{
    if (roomCount >= MAX_ROOMS)
        return false;

    Room r = room;

    if (r.id == 0)
        r.id = nextId++;

    rooms[roomCount++] = r;

    return save();
}

bool RoomRepository::update(const Room& room)
{
    Room* r = get(room.id);

    if (!r)
        return false;

    *r = room;

    return save();
}

bool RoomRepository::remove(uint16_t id)
{
    for (uint8_t i = 0; i < roomCount; i++)
    {
        if (rooms[i].id == id)
        {
            for (uint8_t j = i; j < roomCount - 1; j++)
                rooms[j] = rooms[j + 1];

            roomCount--;

            return save();
        }
    }

    return false;
}

bool RoomRepository::save()
{
    File f = FileStorage::open(Paths::Rooms, "w");

    if (!f)
        return false;

    JsonDocument doc;

    JsonArray arr = doc.to<JsonArray>();

    for (uint8_t i = 0; i < roomCount; i++)
    {
        JsonObject o = arr.add<JsonObject>();

        o["id"] = rooms[i].id;
        o["name"] = rooms[i].name;
        o["icon"] = rooms[i].icon;
        o["enabled"] = rooms[i].enabled;
        o["favorite"] = rooms[i].favorite;
    }

    serializeJson(doc, f);

    f.close();

    return true;
}

bool RoomRepository::load()
{
    roomCount = 0;
    nextId = 1;

    if (!FileStorage::exists(Paths::Rooms))
        return true;

    File f = FileStorage::open(Paths::Rooms, "r");

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
        if (roomCount >= MAX_ROOMS)
            break;

        Room& room = rooms[roomCount++];

        room.id = o["id"] | nextId;
        room.name = o["name"] | "";
        room.icon = o["icon"] | "";
        room.enabled = o["enabled"] | true;
        room.favorite = o["favorite"] | false;

        if (room.id >= nextId)
            nextId = room.id + 1;
    }

    f.close();

    return true;
}
