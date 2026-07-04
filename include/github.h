#pragma once

#include <Arduino.h>

class GitHub
{
public:
    GitHub();

    bool checkVersion();

    String latestVersion();
    String firmwareURL();

private:
    String _latestVersion;
};
