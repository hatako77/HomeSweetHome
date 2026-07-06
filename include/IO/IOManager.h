#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <Arduino.h>

#include "IO/IOConfig.h"
#include "IO/IIODriver.h"
#include "IO/PCF8574Driver.h"


class IOManager
{
public:
    static constexpr uint8_t MAX_IO = 64;
    bool registerChannel(const IOChannel& channel);
    bool write(uint8_t id, bool state);    
    bool read(uint8_t id) const;    
    bool on(uint8_t id);    
    bool off(uint8_t id);    
    bool toggle(uint8_t id);
    void begin();
    void update();
    IOChannel* getChannel(uint8_t id);
    uint8_t count() const;
    IIODriver* getDriver(uint8_t driverId);
private:
static constexpr uint8_t MAX_DRIVERS = 8;
IIODriver* drivers[MAX_DRIVERS];
uint8_t driverCount = 0;
PCF8574Driver pcfDriver;
IOChannel channels[MAX_IO];
    uint8_t ioCount = 0;
};
extern IOManager ioManager;
#endif
