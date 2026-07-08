#include "Core/DemoData.h"

#include "Repositories/RoomRepository.h"
#include "IO/IOManager.h"

void DemoData::create()
{
    if (roomRepository.count() == 0)
    {
        roomRepository.add({0,"Living Room"});
        roomRepository.add({0,"Kitchen"});
        roomRepository.add({0,"Master Bedroom"});
        roomRepository.add({0,"Bedroom"});
        roomRepository.add({0,"Bathroom"});
        roomRepository.add({0,"WC"});
        roomRepository.add({0,"Hallway"});
        roomRepository.add({0,"Yard"});

        roomRepository.save();
    }

    if (ioManager.count() == 0)
        return;

    bool assigned = false;

    for(uint16_t i=0;i<ioManager.count();i++)
    {
        IOChannel* ch = ioManager.getAt(i);

        if(ch && ch->roomId!=0)
        {
            assigned=true;
            break;
        }
    }

    if(assigned)
        return;

    const char* names[] =
    {
        "Main Light",
        "TV Light",
        "Chandelier",
        "Wall Light",

        "Kitchen Main",
        "Cabinet",
        "Hood",

        "Master Main",
        "Bed Light",
        "Reading",

        "Bedroom Main",
        "Desk",

        "Bathroom",

        "WC",

        "Hall",

        "Yard",
        "Parking",
        "Gate",
        "Garden",
        "Facade"
    };

    const uint8_t rooms[] =
    {
        1,1,1,1,
        2,2,2,
        3,3,3,
        4,4,
        5,
        6,
        7,
        8,8,8,8,8
    };

    uint16_t count = sizeof(names)/sizeof(names[0]);

    for(uint16_t i=0;i<count && i<ioManager.count();i++)
    {
        IOChannel* ch = ioManager.getAt(i);

        ch->name=names[i];
        ch->roomId=rooms[i];
    }

    ioManager.save();
}
