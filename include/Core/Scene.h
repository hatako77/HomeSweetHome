#ifndef SCENE_H
#define SCENE_H

#include <Arduino.h>
#include "Core/SceneAction.h"

class Scene
{
public:

    static constexpr uint8_t MAX_ACTIONS = 64;

    uint16_t id = 0;

    String name;

    bool enabled = true;

    uint8_t actionCount = 0;

    SceneAction actions[MAX_ACTIONS];
};

#endif
