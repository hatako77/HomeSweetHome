#include "Room/RoomManager.h"


RoomManager roomManager;


void RoomManager::begin()
{
    roomCount = 0;
    nextId = 1;
}

bool RoomManager::update(const Room& room)
{
    for (uint8_t i = 0; i < roomCount; i++)
    {
        if (rooms[i].id == room.id)
        {
            rooms[i] = room;
            return true;
        }
    }

    return false;
}

uint8_t RoomManager::count() const
{
    return roomCount;
}


Room* RoomManager::get(uint8_t id)
{
    for (uint8_t i = 0; i < roomCount; i++)
    {
        if (rooms[i].id == id)
            return &rooms[i];
    }

    return nullptr;
}

bool RoomManager::add(const Room& room)
{
    if (roomCount >= MAX_ROOMS)
        return false;
    rooms[roomCount] = room;
    rooms[roomCount].id = nextId++;
    roomCount++;
    return true;
}


bool RoomManager::remove(uint8_t id)
{
    if (id >= roomCount)
        return false;


    for (uint8_t i = id; i < roomCount - 1; i++)
    {
        rooms[i] = rooms[i + 1];
        rooms[i].id = i;
    }


    roomCount--;

    return true;
}
