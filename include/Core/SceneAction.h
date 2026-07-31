#ifndef SCENE_ACTION_H
#define SCENE_ACTION_H

#include <Arduino.h>

struct SceneAction
{
    uint16_t channelId = 0;

    bool state = false;

    // Delay before execution
    uint32_t delayMs = 0;

    // How long the state should remain active
    uint32_t durationMs = 0;
};

#endif
