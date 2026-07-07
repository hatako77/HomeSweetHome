#ifndef ROOM_MANAGER_H
#define ROOM_MANAGER_H

#include <Arduino.h>
#include "Room/Room.h"

class RoomManager
{
public:

    static constexpr uint8_t MAX_ROOMS = 32;
    void begin();
    uint8_t count() const;
    Room* get(uint8_t id);
    bool add(const Room& room);
    bool remove(uint8_t id);
    bool update(const Room& room);

private:

    Room rooms[MAX_ROOMS];
    uint8_t roomCount = 0;
    uint16_t nextId = 1;};


extern RoomManager roomManager;


#endif
