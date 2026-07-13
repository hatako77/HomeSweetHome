#include "Core/RoomManager.h"

#include <ArduinoJson.h>
#include <FS.h>

#include "Core/FileStorage.h"
#include "Core/Paths.h"

RoomManager roomManager;

void RoomManager::begin()
{
    load();
}

uint16_t RoomManager::count() const
{
    return roomCount;
}

Room* RoomManager::get(uint16_t id)
{
    for (uint16_t i = 0; i < roomCount; i++)
    {
        if (rooms[i].id == id)
            return &rooms[i];
    }

    return nullptr;
}

Room* RoomManager::add(const Room& room)
{
    if (roomCount >= MAX_ROOMS)
        return nullptr;
    rooms[roomCount] = room;
    rooms[roomCount].id = nextId++;
    roomCount++;
    return &rooms[roomCount - 1];
}


bool RoomManager::update(const Room& room)
{
    Room* existing = get(room.id);

    if (!existing)
        return false;

    *existing = room;

    return save();
}

bool RoomManager::remove(uint16_t id)
{
    for (uint16_t i = 0; i < roomCount; i++)
    {
        if (rooms[i].id == id)
        {
            for (uint16_t j = i; j < roomCount - 1; j++)
            {
                rooms[j] = rooms[j + 1];
            }

            roomCount--;

            return save();
        }
    }

    return false;
}

bool RoomManager::save()
{
    File file = FileStorage::open(Paths::Rooms, "w");

    if (!file)
        return false;

    JsonDocument doc;

    JsonArray array = doc.to<JsonArray>();

    for (uint16_t i = 0; i < roomCount; i++)
    {
        JsonObject obj = array.add<JsonObject>();

        obj["id"] = rooms[i].id;
        obj["name"] = rooms[i].name;
    }

    serializeJson(doc, file);

    file.close();

    return true;
}

bool RoomManager::load()
{
    roomCount = 0;
    nextId = 1;

    if (!FileStorage::exists(Paths::Rooms))
        return true;

    File file = FileStorage::open(Paths::Rooms, "r");

    if (!file)
        return false;

    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, file);

    file.close();

    if (error)
        return false;

    JsonArray array = doc.as<JsonArray>();

    for (JsonObject obj : array)
    {
        if (roomCount >= MAX_ROOMS)
            break;

        Room& room = rooms[roomCount++];

        room.id = obj["id"] | nextId;
        room.name = obj["name"] | "";

        if (room.id >= nextId)
            nextId = room.id + 1;
    }

    return true;
}
