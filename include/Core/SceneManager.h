#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Core/Scene.h"
#include "IO/IOManager.h"

enum class SceneTaskStage : uint8_t
{
    Waiting,
    Restoring
};
struct RunningScene
{
    bool active = false;
    uint16_t sceneId = 0;
    uint32_t startedAt = 0;
    uint32_t totalDuration = 0;
    uint8_t progress = 0;
};

struct SceneTimer
{
    bool active = false;
    SceneTaskStage stage = SceneTaskStage::Waiting;
    uint16_t channelId = 0;
    bool targetState = false;
    bool previousState = false;
    uint32_t durationMs = 0;
    uint32_t expiresAt = 0;
};   // ← این ; خیلی مهمه

class SceneManager
{
public:
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
    void addTimer(uint16_t channelId,bool targetState,bool previousState,uint32_t delayMs,uint32_t durationMs);
    void removeTimers(uint16_t channelId);
    uint32_t calculateDuration(const Scene& scene) const;    
    void startRuntime(const Scene& scene);    
    void updateRuntime();    
    const RunningScene* getRuntime(uint16_t sceneId) const;    
    uint8_t getProgress(uint16_t sceneId) const;
    uint8_t runtimeCount() const;
    RunningScene* getRuntimeAt(uint8_t index);
private:
    static constexpr uint8_t MAX_TIMERS = 32;
    static constexpr uint8_t MAX_RUNNING_SCENES = MAX_SCENES;
    bool hasRunningScenes() const;
    bool updateProgress();
    SceneTimer timers[MAX_TIMERS];
    RunningScene runningScenes[MAX_RUNNING_SCENES];
    Scene scenes[MAX_SCENES];
    uint16_t sceneCount = 0;
    uint16_t nextId = 1;
};

extern SceneManager sceneManager;

#endif
