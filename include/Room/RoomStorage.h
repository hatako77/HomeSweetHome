#ifndef ROOM_STORAGE_H
#define ROOM_STORAGE_H

#include "Repositories/RoomRepository.h"

class RoomStorage
{
public:

    static bool save(RoomRepository& repository);

    static bool load(RoomRepository& repository);
};

#endif
