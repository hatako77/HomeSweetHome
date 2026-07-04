#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include "IOConfig.h"

class IOManager
{
public:

    static const uint8_t TOTAL_IO = 64;

    void begin();

    IOConfig& get(uint8_t index);

    bool getState(uint8_t index);

    void setState(uint8_t index, bool state);

    void toggle(uint8_t index);

private:

    IOConfig io[TOTAL_IO];

};

#endif
