#ifndef OTA_STATUS_H
#define OTA_STATUS_H

#include <Arduino.h>

struct OTAStatus
{
    bool running = false;
    bool finished = false;
    bool success = false;

    uint32_t downloaded = 0;
    uint32_t total = 0;

    uint8_t percent = 0;

    float speedKB = 0;

    uint32_t eta = 0;

    String state = "Idle";
    String error = "";
};

#endif
