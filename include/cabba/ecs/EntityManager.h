#pragma once

#include <cabba/containers/List.h>
#include <cabba/String.h>
#include <cabba/containers/Stack.h>

#include <cabba/ecs/entity.h>
#include <cabba/ecs/Contants.h>

namespace cabba
{
    class World;

    class EntityManager
    {
        int keys = 0;

        public:

            EntityManager(World& game);

            // Get an unused entity 
            Entity* Pool();
            Entity* Pool(const String& name);

            Entity* Get(int index);

            // When you are done with an entity, release it
            void release(Entity* entity);

            class Iterator
            {
            public:

                Iterator(int start, Entity* c, List<int>& rc)
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
                List<int>&		registeredEntities;
                int index = 0;
            };

            EntityManager::Iterator begin()
            {
                return EntityManager::Iterator(0, _entities, registeredEntities);
            }

            EntityManager::Iterator end()
            {
                return EntityManager::Iterator(registeredEntities.size(), _entities, registeredEntities);
            }
            
        private:

            Entity _entities[ENTITY_MAX];

            List<int> registeredEntities;

            // Entities that the user can use
            Stack<Entity*> _entity_pool;
            World& _world;
    };
}