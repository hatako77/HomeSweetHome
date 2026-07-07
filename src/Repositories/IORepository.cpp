#include "Repositories/IORepository.h"

IORepository ioRepository;

void IORepository::begin()
{
}

uint16_t IORepository::count() const
{
    return channelCount;
}

IOChannel* IORepository::get(uint16_t)
{
    return nullptr;
}

bool IORepository::add(const IOChannel&)
{
    return false;
}

bool IORepository::update(const IOChannel&)
{
    return false;
}

bool IORepository::remove(uint16_t)
{
    return false;
}

bool IORepository::load()
{
    return true;
}

bool IORepository::save()
{
    return true;
}
