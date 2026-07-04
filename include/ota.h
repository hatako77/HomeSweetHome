#pragma once

#include <Arduino.h>

class OTA
{
public:
    OTA();

    bool begin(const String &url);

    int progress();
    String status();
    bool running();

private:
    bool updateTask(const String &url);

    static void task(void *parameter);

    volatile int _progress;
    volatile bool _running;

    String _status;
};
