#include <cabba/ecs/EntityManager.h>
#include <cabba/ecs/entity.h>

namespace cabba
{

EntityManager::EntityManager(World& world, int size)
    : _entity_count(size), _entities(new Entity[size]),_world(world)
{
    for (int i=0; i< size; ++i)
    {
        _entities[i].key = i;
        _entities->world = &world;
        _entity_pool.emplace(&_entities[i]);
    }
}
//
//EntityManager::EntityManager(World& systemManager)
//    : _world(systemManager)
//{
//    int k = 0;
//    for (int i=0; i< size; ++i)
//    {
//        _entities[i].key = i;
//        _entities->world = &world;
//        _entity_pool.emplace(&_entities[i]);
//    }
//}

Entity* EntityManager::Pool()
{
    Entity* e = _entity_pool.top();
    registeredEntities.push_back(e->id());
    _entity_pool.pop();
    return e;
}

Entity* EntityManager::Pool(const std::string& str)
{
    Entity* e = _entity_pool.top();
    e->name = str.c_str();
    registeredEntities.push_back(e->id());
    _entity_pool.pop();
    return e;
}

void EntityManager::release(Entity* entity)
{
    for (const std::type_index& typeIndex : entity->registeredComponents())
    {
        for (auto& pair : _world._pools)
        {
            if (pair.first == typeIndex)
            {
                pair.second->unregisterComponent(entity->id());
            }
        }
    }
    registeredEntities.erase(
        std::find(
            registeredEntities.begin(),
            registeredEntities.end(),
            entity->id()));

    _entity_pool.emplace(entity);
}

Entity* EntityManager::Get(int id)
{
    return &_entities[id];
}

int EntityManager::left()const
{
    return _entity_pool.size();
}

int EntityManager::size()const
{
    return _entity_pool.size();
}

}