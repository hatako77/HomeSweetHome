#pragma once

#include <Arduino.h>
#include "IO/IOConfig.h"

class IORepository
{
public:
    static constexpr uint16_t MAX_CHANNELS = 64;
    void begin();
    uint16_t count() const;
    IOChannel* get(uint16_t id);
    bool add(const IOChannel& channel);
    bool update(const IOChannel& channel);
    bool remove(uint16_t id);
    bool load();
    bool save();
private:
    IOChannel channels[MAX_CHANNELS];
    uint16_t channelCount = 0;
    uint16_t nextId = 1;
};
extern IORepository ioRepository;
