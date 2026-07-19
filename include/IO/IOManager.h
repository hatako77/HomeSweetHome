#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <Arduino.h>

#include "IO/IOConfig.h"
#include "IO/IIODriver.h"
#include "IO/PCF8574Driver.h"


class IOManager
{
public:
    uint16_t countByRoom(uint16_t roomId) const;
    static constexpr uint8_t MAX_IO = 64;
    bool write(uint16_t id, bool state);    
    bool read(uint16_t id) const;  
    bool update(const IOChannel& channel);
    void begin();
    void update();
    IOChannel* getChannel(uint16_t id);
    const IOChannel* getChannel(uint16_t id) const;
    uint16_t count() const;
    IIODriver* getDriver(uint16_t driverId);
    bool add(const IOChannel& channel);
    bool remove(uint16_t id);
    bool save() const;
    bool load();
    bool registerDriver(IIODriver* driver);
    bool assignToRoom(uint16_t channelId, uint8_t roomId);
    const IOChannel* getAt(uint16_t index) const;
    IOChannel* getAt(uint16_t index);
private:
    static constexpr uint8_t MAX_DRIVERS = 8;
    IIODriver* drivers[MAX_DRIVERS];
    uint16_t driverCount = 0;
    IOChannel channels[MAX_IO];
    uint16_t channelCount = 0;
    uint16_t nextId = 1;
};
extern IOManager ioManager;
#endif
