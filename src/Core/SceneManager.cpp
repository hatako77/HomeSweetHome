#include "Core/SceneManager.h"
#include "Core/IOManager.h"
#include "Web/Notifier.h"

extern IOManager ioManager;
SceneManager sceneManager;

void SceneManager::begin()
{
}

bool SceneManager::load()
{
    return true;
}

bool SceneManager::save()
{
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

    return true;
}

bool SceneManager::update(const Scene& scene)
{
    Scene* s = get(scene.id);

    if(s == nullptr)
        return false;

    *s = scene;

    save();

    return true;
}

bool SceneManager::remove(uint16_t id)
{
    for(uint16_t i=0;i<sceneCount;i++)
    {
        if(scenes[i].id==id)
        {
            for(uint16_t j=i;j<sceneCount-1;j++)
            {
                scenes[j]=scenes[j+1];
            }

            sceneCount--;

            save();

            return true;
        }
    }

    return false;
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
        const SceneAction& action = scene->actions[i];

        IOChannel* channel =
            ioManager.getChannel(action.channelId);

        if(channel == nullptr)
            continue;

        ioManager.write(
            action.channelId,
            action.state
        );

        channel->state = action.state;

        Notifier::channelChanged(*channel);
    }

    ioManager.save();

    return true;
}
