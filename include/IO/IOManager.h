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
    static constexpr uint16_t MAX_IO = 64;
    bool write(uint16_t id, bool state);    
    bool read(uint16_t id) const;    
    bool on(uint16_t id);    
    bool off(uint16_t id);    
    bool toggle(uint16_t id);
    void begin();
    void update();
    IOChannel* getChannel(uint16_t id);
    const IOChannel* getChannel(uint16_t id) const;
    uint16_t count() const;
    IIODriver* getDriver(uint16_t driverId);
    bool add(const IOChannel& channel);
    bool update(const IOChannel& channel);
    bool remove(uint16_t id);
private:
    static constexpr uint16_t MAX_DRIVERS = 8;
    IIODriver* drivers[MAX_DRIVERS];
    uint16_t driverCount = 0;
    PCF8574Driver pcfDriver;
    IOChannel channels[MAX_IO];
    uint16_t channelCount = 0;
    uint16_t nextId = 1;
};
extern IOManager ioManager;
#endif
