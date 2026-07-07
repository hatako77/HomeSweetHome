#pragma once

#include <Arduino.h>
#include <FS.h>

class FileStorage
{
public:
    static bool exists(const char* path);

    static bool remove(const char* path);

    static File open(const char* path, const char* mode);
};
