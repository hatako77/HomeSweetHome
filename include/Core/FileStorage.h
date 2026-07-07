#pragma once

#include <Arduino.h>

class FileStorage
{
public:
    static bool exists(const char* path);

    static bool remove(const char* path);

    static bool read(
        const char* path,
        uint8_t* buffer,
        size_t size);

    static bool write(
        const char* path,
        const uint8_t* buffer,
        size_t size);
};
