#ifndef SCENE_ACTION_H
#define SCENE_ACTION_H

#include <Arduino.h>

struct SceneAction
{
    uint16_t channelId = 0;
    bool state = false;
};

#endif
