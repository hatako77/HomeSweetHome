#ifndef ROOM_H
#define ROOM_H

#include <Arduino.h>

struct Room
{
    uint8_t id;

    String name;

    String icon;

    bool enabled;

    bool favorite;
};
bool update(const Room& room);
#endif
