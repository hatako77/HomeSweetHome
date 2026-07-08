#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <Arduino.h>

#include "IO/IOConfig.h"
#include "IO/IIODriver.h"
#include "IO/PCF8574Driver.h"


class IOManager
{
public:
    uint8_t countByRoom(uint8_t roomId) const;
    static constexpr uint8_t MAX_IO = 64;
    bool write(uint8_t id, bool state);    
    bool read(uint8_t id) const;    
    bool on(uint8_t id);    
    bool off(uint8_t id);    
    bool toggle(uint8_t id);
    void begin();
    void update();
    IOChannel* getChannel(uint8_t id);
    const IOChannel* getChannel(uint8_t id) const;
    uint8_t count() const;
    IIODriver* getDriver(uint8_t driverId);
    bool add(const IOChannel& channel);
    bool update(const IOChannel& channel);
    bool remove(uint16_t id);
private:
    static constexpr uint8_t MAX_DRIVERS = 8;
    IIODriver* drivers[MAX_DRIVERS];
    uint8_t driverCount = 0;
    PCF8574Driver pcfDriver;
    IOChannel channels[MAX_IO];
    uint16_t channelCount = 0;
    uint16_t nextId = 1;
};
extern IOManager ioManager;
#endif
