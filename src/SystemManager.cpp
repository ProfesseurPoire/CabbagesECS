#include <cabba/ecs/SystemManager.h>
#include <cabba/ecs/System.h>

namespace cabba
{

SystemManager::~SystemManager()
{
    Clear();
}

void SystemManager::Clear()
{
    for (auto* system : systems)
    {
        delete system;
    }

    systems.clear();
}

void SystemManager::Add(SystemInterface* system)
{
    systems.push_back(system);
}

void SystemManager::Update(World& game, float elapsedTime)
{
    for (auto* system : systems)
    {
        system->Update(game, elapsedTime);
    }
}
}