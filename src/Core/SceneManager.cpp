#include "Core/SceneManager.h"
#include "IO/IOManager.h"
#include "Web/Notifier.h"
#include "Core/FileStorage.h"
#include "Core/Paths.h"


extern IOManager ioManager;
SceneManager sceneManager;

void SceneManager::begin()
{
}
void SceneManager::update()
{
    uint32_t now = millis();

    for(auto& timer : timers)
    {
        if(!timer.active)
            continue;

        if((int32_t)(now - timer.expiresAt) < 0)
            continue;

        switch(timer.stage)
        {
            //--------------------------------------------------
            // Delay finished
            //--------------------------------------------------
            case SceneTaskStage::Waiting:
            {
                ioManager.write(
                    timer.channelId,
                    timer.targetState);

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

            //--------------------------------------------------
            // Duration finished
            //--------------------------------------------------
            case SceneTaskStage::Restoring:
            {
                ioManager.write(
                    timer.channelId,
                    timer.previousState);

                timer.active = false;

                break;
            }
        }
    }
}
void SceneManager::addTimer(uint16_t channelId,bool targetState,bool previousState,uint32_t delayMs,uint32_t durationMs)
{
    uint32_t expiresAt = millis() + delayMs;
    // اگر قبلاً برای این کانال تایمر وجود دارد
    for(auto& timer : timers)
    {
        if(timer.active && timer.channelId == channelId)
        {
            timer.stage = SceneTaskStage::Waiting;
            timer.targetState = targetState;
            timer.previousState = previousState;
            timer.durationMs = durationMs;
            timer.expiresAt = expiresAt;
            return;
        }
    }
    // پیدا کردن اولین خانه آزاد
    for(auto& timer : timers)
    {
        if(!timer.active)
        {
            timer.active = true;
            timer.stage = SceneTaskStage::Waiting;
            timer.channelId = channelId;
            timer.targetState = targetState;
            timer.previousState = previousState;
            timer.durationMs = durationMs;
            timer.expiresAt = expiresAt;
            return;
        }
    }
}

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
            a.durationMs   = action["durationMs"] | 0;
            a.channelId = action["channelId"] | 0;
            a.state     = action["state"] | false;
        }
        if (scene.id >= nextId)
            nextId = scene.id + 1;
    }
    return true;
}

Scene* SceneManager::getAt(uint16_t index)
{
    if(index>=sceneCount)
        return nullptr;
    return &scenes[index];
}

const Scene* SceneManager::getAt(uint16_t index) const
{
    if(index>=sceneCount)
        return nullptr;
    return &scenes[index];
}

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
            JsonObject a   = actions.add<JsonObject>();
            a["channelId"] = scenes[i].actions[j].channelId;
            a["state"]     = scenes[i].actions[j].state;
            a["durationMs"]   = scenes[i].actions[j].durationMs;
        }
    }
    serializeJson(doc,file);
    file.close();
    return true;
}

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

bool SceneManager::update(const Scene& scene)
{
    Scene* s = get(scene.id);
    if(s == nullptr) return false;
    *s = scene;
    save();
    Notifier::sceneUpdated(*s);
    return true;
}



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

void SceneManager::removeTimers(uint16_t channelId)
{
    for(auto& timer : timers)
    {
        if(timer.active && timer.channelId == channelId)
        {
            timer.active = false;
        }
    }
}

Scene* SceneManager::get(uint16_t id)
{
    for(uint16_t i = 0; i < sceneCount; i++)
    {
        if(scenes[i].id == id)
            return &scenes[i];
    }

    return nullptr;
}

const Scene* SceneManager::get(uint16_t id) const
{
    for(uint16_t i = 0; i < sceneCount; i++)
    {
        if(scenes[i].id == id)
            return &scenes[i];
    }

    return nullptr;
}

uint16_t SceneManager::count() const
{
    return sceneCount;
}

bool SceneManager::execute(uint16_t id)
{
    Scene* scene = get(id);
    if(scene == nullptr)
        return false;

    for(uint8_t i = 0; i < scene->actionCount; i++)
    {
        SceneAction& action = scene->actions[i];

        IOChannel* channel = ioManager.getChannel(action.channelId);
        if(channel == nullptr)
            continue;

        // لغو هر Scene قبلی روی این کانال
        removeTimers(action.channelId);

        bool previousState = channel->state;

        //--------------------------------------------------
        // اجرای فوری بدون هیچ تایمری
        //--------------------------------------------------
        if(action.delayMs == 0 &&
           action.durationMs == 0)
        {
            ioManager.write(
                action.channelId,
                action.state,
                true);

            continue;
        }

        //--------------------------------------------------
        // زمان‌بندی اجرای اکشن
        //--------------------------------------------------
        addTimer(
            action.channelId,
            action.state,
            previousState,
            action.delayMs,
            action.durationMs);
    }

    ioManager.save();

    Notifier::sceneExecuted(id);

    return true;
}
