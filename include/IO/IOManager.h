#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <Arduino.h>

#include "IO/IOConfig.h"
#include "IO/PCF8574Driver.h"

class IOManager
{
public:

    static const uint8_t MAX_IO = 64;
    bool write(uint8_t id, bool state);
    
    bool read(uint8_t id);
    
    bool on(uint8_t id);
    
    bool off(uint8_t id);
    
    bool toggle(uint8_t id);
    void begin();

    void update();

bool setState(uint8_t id, bool state);
bool getState(uint8_t id);
    IOChannel* get(uint8_t id);

    PCF8574Driver& getDriver();
    uint8_t count();

private:

    PCF8574Driver driver;

    IOChannel channels[MAX_IO];

    uint8_t ioCount = 0;
};

extern IOManager ioManager;

#endif
