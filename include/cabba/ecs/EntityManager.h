#pragma once

#include <cabba/ecs/entity.h>

#include <vector>
#include <string>
#include <stack>

namespace cabba
{
class World;

class EntityManager
{
    int keys = 0;

public:
    
   // EntityManager(World& game);

    /*
     * @brief   Builds a new Entity Manager object that will contains
     */
    EntityManager(World& game, int size);

    /*
     * @brief   Returns how many entity the user can pool from the manager
     */
    int left()const;

    /*
     * @brief   Returns how many entity the manager owns
     */
    int size()const;

    // Get an unused entity 
    Entity* Pool();
    Entity* Pool(const std::string& name);

    Entity* Get(int index);

    // When you arCe done with an entity, release it
    void release(Entity* entity);

    class Iterator
    {
    public:

        Iterator(int start, Entity* c, std::vector<int>& rc)
            : entities(c), registeredEntities(rc), index(start)
        {}

        Entity& operator->()const noexcept
        {
            return entities[registeredEntities[index]];
        }

        Entity& operator*()const
        {
            return entities[registeredEntities[index]];
        }

        void operator++()
        {
            index++;
        }

        bool operator!=(const Iterator& it)const
        {
            return index != it.index;
        }

        Entity*		entities;
        std::vector<int>&		registeredEntities;
        int index = 0;
    };

    Iterator begin()
    {
        return Iterator(0, _entities, registeredEntities);
    }

    Iterator end()
    {
        return Iterator(registeredEntities.size(), _entities, registeredEntities);
    }

private:

    Entity* _entities;
    int _entity_count = 0;

    std::vector<int> registeredEntities;

    // Entities that the user can use
    std::stack<Entity*> _entity_pool;
    World& _world;
};
}