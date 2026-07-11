#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Core/Scene.h"
#include "IO/IOManager.h"

struct SceneTimer
{
    bool active = false;
    uint16_t channelId = 0;
    bool targetState = false;
    uint32_t expiresAt = 0;
};

class SceneManager
{
public:
    void removeTimer(uint16_t channelId);
    static constexpr uint8_t MAX_SCENES = 32;
    void begin();
    bool load();
    bool save();
    bool saveScene(Scene& scene);
    bool add(const Scene& scene);
    bool update(const Scene& scene);
    bool remove(uint16_t id);
    Scene* get(uint16_t id);
    const Scene* get(uint16_t id) const;
    uint16_t count() const;
    bool execute(uint16_t id);
    Scene* getAt(uint16_t index);    
    const Scene* getAt(uint16_t index) const;
    void update();    
    void addTimer(uint16_t channelId,bool targetState,uint32_t durationMs);

private:
    static constexpr uint8_t MAX_TIMERS = 32;    
    SceneTimer timers[MAX_TIMERS];
    Scene scenes[MAX_SCENES];
    uint16_t sceneCount = 0;
    uint16_t nextId = 1;
};

extern SceneManager sceneManager;

#endif
