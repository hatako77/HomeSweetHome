#include "Core/SceneManager.h"

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

bool SceneManager::execute(uint16_t)
{
    return false;
}
