#ifndef ROOM_REPOSITORY_H
#define ROOM_REPOSITORY_H

#include <Arduino.h>
#include "Room/Room.h"

class RoomRepository
{
public:

    static constexpr uint8_t MAX_ROOMS = 32;
    void begin();
    uint8_t count() const;
    Room* get(uint16_t id);
    bool add(const Room& room);
    bool remove(uint16_t id);
    bool update(const Room& room);
    bool load();
    bool save();
private:

    Room rooms[MAX_ROOMS];
    uint8_t roomCount = 0;
    uint16_t nextId = 1;};


extern RoomRepository roomRepository;


#endif
