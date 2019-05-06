#include <cabba/ecs/EntityManager.h>
#include <cabba/ecs/Entity.h>

namespace cabba
{
EntityPool::EntityPool(World& world, int size)
    : _entities(new Entity[size]),_world(world)
{
    for (int i=0; i< size; ++i)
    {
        _entities[i].key = i;
        _entities[i].world = &world;
        _entity_pool.emplace(&_entities[i]);
    }
}

EntityPool::~EntityPool()
{
    delete[] _entities;
}

Entity& EntityPool::get()
{
    Entity* e = _entity_pool.top();
    _registered_entities.push_back(e->id());
    _entity_pool.pop();
    return *e;
}

Entity* EntityPool::get(const int id)
{
    return &_entities[id];
}

void EntityPool::release(Entity* entity)
{
    _released_entities.push_back(entity);
}

int EntityPool::left()const
{
    return _entity_pool.size();
}

int EntityPool::size()const
{
    return _entity_pool.size();
}

void EntityPool::release_immediate(Entity* e)
{
    //// Making sure to also delete the components I guess
    //for (auto& pair : _world.component_manager()._pools)
    //{
    //    // Check if there's a component associated to the entity
    //    if(pair.second->exist(e->id()))
    //    {
    //        // Then we remove the component from the entity
    //        pair.second->remove(e->id());
    //    }
    //}

    //_registered_entities.remove(e->id());
    //_entity_pool.emplace(e);
}

void EntityPool::finalize_release()
{
    for (int i = 0; i < _released_entities.size(); ++i)
    {
        _world.release_entity_immediate(_released_entities[i]);
    }
    _released_entities.clear();
}

int EntityPool::used()const { return _registered_entities.size();}

}
