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

bool SceneManager::add(const Scene&)
{
    return false;
}

bool SceneManager::update(const Scene&)
{
    return false;
}

bool SceneManager::remove(uint16_t)
{
    return false;
}

Scene* SceneManager::get(uint16_t)
{
    return nullptr;
}

const Scene* SceneManager::get(uint16_t) const
{
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
