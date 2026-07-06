#ifndef IO_STORAGE_H
#define IO_STORAGE_H
class IOManager;

class IOStorage
{
public:
static bool load(IOManager& manager);
static bool save(const IOManager& manager);
};

#endif
