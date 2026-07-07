#include "Room/RoomManager.h"


RoomManager roomManager;


void RoomManager::begin()
{
    roomCount = 0;
}

bool RoomManager::update(const Room& room)
{
    if (room.id >= roomCount)
        return false;

    rooms[room.id] = room;

    return true;
}
uint8_t RoomManager::count() const
{
    return roomCount;
}


Room* RoomManager::get(uint8_t id)
{
    if (id >= roomCount)
        return nullptr;

    return &rooms[id];
}


bool RoomManager::add(const Room& room)
{
    if (roomCount >= MAX_ROOMS)
        return false;


    rooms[roomCount] = room;
    rooms[roomCount].id = roomCount;

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
