#ifndef IO_STORAGE_H
#define IO_STORAGE_H

class IOStorage
{
public:
static bool load(IOManager& manager);
static bool save(const IOManager& manager);
};

#endif
