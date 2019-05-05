#include <cabba/ecs/SystemManager.h>
#include <cabba/ecs/System.h>

namespace cabba
{
SystemMap::~SystemMap(){ clear();}

int SystemMap::size() const
{
    return _systems.size();
}

void SystemMap::clear()
{
    for (auto& system : _systems)
        delete system.second;
    _systems.clear();
}

bool SystemMap::empty() const
{
    return _systems.size()==0;
}

void SystemMap::update(World& game, float elapsedTime)
{
    for (auto& system : _systems)
        system.second->update(game, elapsedTime);
}
}