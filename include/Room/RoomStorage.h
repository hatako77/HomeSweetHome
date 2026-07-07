#ifndef ROOM_STORAGE_H
#define ROOM_STORAGE_H

#include "Repositories/RoomRepository.h"

class RoomStorage
{
public:

    static bool save(RoomManager& manager);

    static bool load(RoomManager& manager);
};

#endif
