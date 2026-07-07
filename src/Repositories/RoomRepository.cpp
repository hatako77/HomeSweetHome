#include "Repositories/RoomRepository.h"
RoomRepository roomRepository;
bool RoomRepository::add(const Room&)
{
    return false;
}
 
bool RoomRepository::update(const Room&)
{
    return false;
}

bool RoomRepository::remove(uint16_t)
{
    return false;
}

Room* RoomRepository::get(uint16_t)
{
    return nullptr;
}
uint8_t RoomRepository::count() const
{
    return roomCount;
}
