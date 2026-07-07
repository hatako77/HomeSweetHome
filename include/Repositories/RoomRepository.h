#pragma once

#include "Core/IRepository.h"
#include "Room/Room.h"

class RoomRepository : public IRepository<Room>
{
public:
    bool add(const Room& room) override;
    bool update(const Room& room) override;
    bool remove(uint16_t id) override;
    Room* get(uint16_t id) override;
};
