#include <cabba/ecs/SystemManager.h>
#include <cabba/ecs/System.h>

namespace cabba
{
SystemManager::~SystemManager(){ clear();}

int SystemManager::size() const
{
    return _systems.size();
}

void SystemManager::clear()
{
    for (auto& system : _systems)
        delete system.second;
    _systems.clear();
}

bool SystemManager::empty() const
{
    return _systems.size()==0;
}

void SystemManager::update(World& game, float elapsedTime)
{
    for (auto& system : _systems)
        system.second->update(game, elapsedTime);
}
}