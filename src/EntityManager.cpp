#include "EntityManager.h"

#include <cabba/ecs/entity.h>

namespace cabba
{
    EntityManager::EntityManager(World& systemManager)
        : _world(systemManager)
    {
        int k = 0;
        for (auto& entity : _entities)
        {
            // This is bad but apparently I can't get
            // into the copy constructor so :shrug:

            entity.key = k++;
            entity.world = &systemManager;
            _entity_pool.add(&entity);
        }
    }

    Entity* EntityManager::Pool()
    {
        Entity* e = _entity_pool.peek();
        registeredEntities.add(e->id());
        _entity_pool.pop();
        return e;
    }

    Entity* EntityManager::Pool(const String& str)
    {
        Entity* e = _entity_pool.peek();
        e->name = str.cstr();
        registeredEntities.add(e->id());
        _entity_pool.pop();
        return e;
    }

    void EntityManager::release(Entity* entity)
    {
        for (const std::type_index& typeIndex : entity->registeredComponents())
        {
            for(auto& pair : _world._pools)
            {
                if (pair.first == typeIndex)
                {
                    pair.second->unregisterComponent(entity->id());
                }
            }
        }
        registeredEntities.remove(entity->id());
        _entity_pool.add(entity);
    }

    Entity* EntityManager::Get(int id)
    {
        return &_entities[id];
    }
}