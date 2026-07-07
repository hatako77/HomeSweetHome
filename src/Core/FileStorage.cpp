#include "Core/FileStorage.h"

#include <LittleFS.h>

bool FileStorage::exists(const char* path)
{
    return LittleFS.exists(path);
}

bool FileStorage::remove(const char* path)
{
    return LittleFS.remove(path);
}

File FileStorage::open(const char* path, const char* mode)
{
    return LittleFS.open(path, mode);
}
