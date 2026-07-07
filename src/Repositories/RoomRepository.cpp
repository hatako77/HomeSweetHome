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


bool RoomRepository::load()
{
        if (!FileStorage::exists(Paths::Rooms))
        return false;
    File f = FileStorage::open(Paths::Rooms, "r");
    if (!f)
        return false;
    JsonDocument doc;
    if (deserializeJson(doc, f))
    {
        f.close();
        return false;
    }
    JsonArray arr = doc.as<JsonArray>();
    for (JsonObject o : arr)
    {
        Room room;
        room.id = o["id"] | 0;
        room.name = o["name"] | "Room";
        room.icon = o["icon"] | "home";
        room.enabled = o["enabled"] | true;
        room.favorite = o["favorite"] | false;
        repository.add(room);
    }
    f.close();
    return true;
}
