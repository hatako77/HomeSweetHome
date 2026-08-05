#include "Core/SceneManager.h"
#include "IO/IOManager.h"
#include "Web/Notifier.h"
#include "Core/FileStorage.h"
#include "Core/Paths.h"

extern IOManager ioManager;
SceneManager sceneManager;
//===========================================================================
void SceneManager::begin()
{
    load();
}
//===========================================================================
void SceneManager::update()
{
    uint32_t now = millis();
    for(auto& timer : timers)
    {
        if(!timer.active)continue;
        if((int32_t)(now - timer.expiresAt) < 0)continue;
        switch(timer.stage)
        {
            case SceneTaskStage::Waiting:
            {
                ioManager.write(timer.channelId,timer.targetState,true);
                if(timer.durationMs == 0)
                {
                    timer.active = false;
                }
                else
                {
                    timer.stage = SceneTaskStage::Restoring;
                    timer.expiresAt = now + timer.durationMs;
                }
                break;
            }
            case SceneTaskStage::Restoring:
            {
                ioManager.write(timer.channelId,timer.previousState,true);
                timer.active = false;
                break;
            }
        }
        static uint32_t lastProgressUpdate = 0;
        if (millis() - lastProgressUpdate >= 500)
        {
            lastProgressUpdate = millis();        
            updateProgress();        
            Notifier::sceneProgressChanged();
        }
    }
}
//===========================================================================
void SceneManager::updateProgress()
{
    uint32_t now = millis();
    for (auto &rt : runtimes)
    {
        if (!rt.active) continue;
        uint32_t elapsed = now - rt.startTime;
        if (elapsed >= rt.totalDuration)
        {
            rt.progress = 100;
            rt.active = false;
            continue;
        }
        rt.progress = (elapsed * 100) / rt.totalDuration;
    }
}
//===========================================================================
void SceneManager::addTimer(uint16_t channelId,bool targetState,bool previousState,uint32_t delayMs,uint32_t durationMs)
{
    uint32_t now = millis();
    for(auto& timer : timers)
    {
        if(timer.active && timer.channelId == channelId)
        {
            timer.active        = true;
            timer.stage         = SceneTaskStage::Waiting;
            timer.channelId     = channelId;
            timer.targetState   = targetState;
            timer.previousState = previousState;
            timer.durationMs    = durationMs;
            timer.expiresAt     = now + delayMs;
            return;
        }
    }
    for(auto& timer : timers)
    {
        if(!timer.active)
        {
            timer.active        = true;
            timer.stage         = SceneTaskStage::Waiting;
            timer.channelId     = channelId;
            timer.targetState   = targetState;
            timer.previousState = previousState;
            timer.durationMs    = durationMs;
            timer.expiresAt     = now + delayMs;
            return;
        }
    }
    Serial.println("No free Scene timers.");
}
//===========================================================================
bool SceneManager::saveScene(Scene& scene)
{
    if(scene.id == 0)
    {
        scene.id = nextId++;
        if(sceneCount >= MAX_SCENES) return false;
        scenes[sceneCount++] = scene;
        save();
        Notifier::sceneAdded(scene);
        return true;
    }
    Scene* existing = get(scene.id);
    if(existing == nullptr) return false;
    *existing = scene;
    save();
    Notifier::sceneUpdated(scene);
    return true;
}
//===========================================================================
bool SceneManager::load()
{
    sceneCount = 0;
    nextId = 1;
    if (!FileStorage::exists(Paths::Scenes))
        return true;
    File file = FileStorage::open(Paths::Scenes, "r");
    if (!file)
        return false;
    JsonDocument doc;
    DeserializationError error =
        deserializeJson(doc, file);
    file.close();
    if (error) return false;
    JsonArray scenesArray = doc.as<JsonArray>();
    for (JsonObject sceneObj : scenesArray)
    {
        if (sceneCount >= MAX_SCENES) break;
        Scene& scene = scenes[sceneCount++];
        scene.id = sceneObj["id"] | nextId;
        scene.name = sceneObj["name"] | "";
        scene.enabled = sceneObj["enabled"] | true;
        scene.actionCount = 0;
        scene.icon = sceneObj["icon"] | "bolt";        
        scene.favorite = sceneObj["favorite"] | false;
        scene.notificationSend=sceneObj["notificationSend"]|false;
        strlcpy(scene.notificationText,sceneObj["notificationText"]|"",sizeof(scene.notificationText));
        JsonArray actions = sceneObj["actions"].as<JsonArray>();
        for (JsonObject action : actions)
        {
            if (scene.actionCount >= Scene::MAX_ACTIONS) break;
            SceneAction& a = scene.actions[scene.actionCount++];
            a.delayMs    = action["delayMs"] | 0;
            a.durationMs = action["durationMs"] | 0;
            a.durationMs = action["durationMs"] | 0;
            a.channelId  = action["channelId"] | 0;
            a.state      = action["state"] | false;
        }
        if (scene.id >= nextId)
            nextId = scene.id + 1;
    }
    return true;
}
//===========================================================================
Scene* SceneManager::getAt(uint16_t index)
{
    if(index>=sceneCount)
        return nullptr;
    return &scenes[index];
}
//===========================================================================
const Scene* SceneManager::getAt(uint16_t index) const
{
    if(index>=sceneCount)
        return nullptr;
    return &scenes[index];
}
//===========================================================================
bool SceneManager::save()
{
    File file = FileStorage::open(Paths::Scenes, "w");
    if (!file)
        return false;
    JsonDocument doc;
    JsonArray scenesArray = doc.to<JsonArray>();
    for (uint16_t i = 0; i < sceneCount; i++)
    {
        JsonObject sceneObj = scenesArray.add<JsonObject>();
        sceneObj["id"]   = scenes[i].id;
        sceneObj["name"] = scenes[i].name;        
        sceneObj["icon"] = scenes[i].icon;        
        sceneObj["favorite"] = scenes[i].favorite;        
        sceneObj["enabled"] = scenes[i].enabled;  
        sceneObj["notificationSend"] = scenes[i].notificationSend;  
        sceneObj["notificationText"] = scenes[i].notificationText;        
        JsonArray actions = sceneObj["actions"].to<JsonArray>();
        for(uint8_t j=0; j<scenes[i].actionCount; j++)
        {
            JsonObject a      = actions.add<JsonObject>();
            a["channelId"]    = scenes[i].actions[j].channelId;
            a["state"]        = scenes[i].actions[j].state;
            a["delayMs"]      = scenes[i].actions[j].delayMs;
            a["durationMs"]   = scenes[i].actions[j].durationMs;
        }
    }
    serializeJson(doc,file);
    file.close();
    return true;
}
//===========================================================================
bool SceneManager::add(const Scene& scene)
{
    if(sceneCount >= MAX_SCENES)
        return false;
    if(get(scene.id) != nullptr)
        return false;
    scenes[sceneCount++] = scene;
    save();
    Notifier::sceneAdded(scene);
    return true;
}
//===========================================================================
bool SceneManager::update(const Scene& scene)
{
    Scene* s = get(scene.id);
    if(s == nullptr) return false;
    *s = scene;
    updateRuntime();
    save();
    Notifier::sceneUpdated(*s);
    return true;
}
//===========================================================================
bool SceneManager::remove(uint16_t id)
{
    for(uint16_t i = 0; i < sceneCount; i++)
    {
        if(scenes[i].id == id)
        {
            for(uint16_t j = i;j < sceneCount - 1;j++)
            {
                scenes[j] = scenes[j + 1];
            }
            sceneCount--;
            save();
            Notifier::sceneRemoved(id);
            return true;
        }
    }
    return false;
}
//===========================================================================
void SceneManager::removeTimers(uint16_t channelId)
{
    for(auto& timer : timers)
    {
        if(timer.active && timer.channelId == channelId) timer.active = false;
    }
}
//===========================================================================
Scene* SceneManager::get(uint16_t id)
{
    for(uint16_t i = 0; i < sceneCount; i++)
    {
        if(scenes[i].id == id) return &scenes[i];
    }
    return nullptr;
}
//===========================================================================
const Scene* SceneManager::get(uint16_t id) const
{
    for(uint16_t i = 0; i < sceneCount; i++)
    {
        if(scenes[i].id == id) return &scenes[i];
    }
    return nullptr;
}
//===========================================================================
uint16_t SceneManager::count() const
{
    return sceneCount;
}
//===========================================================================
bool SceneManager::execute(uint16_t id)
{
    Scene* scene = get(id);
    if(scene == nullptr) return false;
    for(uint8_t i = 0; i < scene->actionCount; i++)
    {
        SceneAction& action = scene->actions[i];
        IOChannel* channel = ioManager.getChannel(action.channelId);
        if(channel == nullptr) continue;
        bool previousState = channel->state;
        removeTimers(action.channelId);
        if(action.delayMs == 0)
        {
            ioManager.write(action.channelId,action.state,true);
            // اگر Duration دارد، فقط تایمر بازگشت بساز
            if(action.durationMs > 0) addTimer(action.channelId,action.state,previousState,0,action.durationMs);
            continue;
        }
        addTimer(action.channelId,action.state,previousState,action.delayMs,action.durationMs);
    }
    startRuntime(*scene);
    ioManager.save();
    Notifier::sceneExecuted(id);
    return true;
}
//===========================================================================
uint32_t SceneManager::calculateDuration(const Scene& scene) const
{
    uint32_t maxDuration = 0;
    for(uint8_t i=0;i<scene.actionCount;i++)
    {
        uint32_t t = scene.actions[i].delayMs + scene.actions[i].durationMs;
        if(t > maxDuration) maxDuration = t;
    }
    return maxDuration;
}
//===========================================================================
void SceneManager::startRuntime(const Scene& scene)
{
    RunningScene* slot = nullptr;
    for(auto& s : runningScenes)
    {
        if(s.active && s.sceneId == scene.id)
        {
            slot = &s;
            break;
        }
        if(!slot && !s.active) slot = &s;
    }
    if(!slot) return;
    slot->active = true;
    slot->sceneId = scene.id;
    slot->startedAt = millis();
    slot->totalDuration = calculateDuration(scene);
}
//===========================================================================
void SceneManager::updateRuntime()
{
    uint32_t now = millis();
    for(auto& s : runningScenes)
    {
        if(!s.active) continue;
        if(now - s.startedAt >= s.totalDuration) s.active = false;
    }
}
//===========================================================================
const RunningScene* SceneManager::getRuntime(uint16_t sceneId) const
{
    for(const auto& s : runningScenes)
    {
        if(s.active && s.sceneId == sceneId) return &s;
    }
    return nullptr;
}
//===========================================================================
uint8_t SceneManager::getProgress(uint16_t sceneId) const
{
    const RunningScene* s = getRuntime(sceneId);
    if(!s) return 0;
    if(s->totalDuration == 0) return 100;
    uint32_t elapsed = millis() - s->startedAt;
    if(elapsed >= s->totalDuration) return 100;
    return (elapsed * 100UL) / s->totalDuration;
}
