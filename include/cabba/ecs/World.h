#pragma once

#include <cabba/ecs/EntityManager.h>
#include <cabba/ecs/ComponentPool.h>
#include <cabba/ecs/OwningPointer.h>

#include <cabba/ecs/PolymorphicMap.h>
#include <cabba/ecs/System.h>

#include <memory>
#include <iostream>


namespace cabba
{
    // If a pool of component doesn't exist, it will get
    // initialized. Thus I advise you to initialize every 
    // pool at the start of the game to avoid random
    // memory allocation when the game started
    class World
    {
    public:

        /*!
         * @brief   Construct the world object and the EntityPool and 
         *          component manager using the given parameters
         */
        World(const int entity_count) 
            : _entity_manager(*this, entity_count){}
       
        /*Entity& get_entity()
        {
            return *_entity_manager.get();
        }*/

        /*!
         * @brief   Returns a reference to the entity manager
         */
        EntityPool& entity_pool() { return _entity_manager; }

        void release_entity_immediate(Entity* e);

        /*!
         * @brief   Returns a pool of type T. Must have been initialized before
         */
        template<class T>
        ObserverPointer<ComponentPool<T>> get_component_pool()
        {
            return _component_pools.get<ComponentPool<T>>();
        }

        /*!
         * @brief   Add a pool of type T
         * 
         *          Only 1 pool of 1 type can exist at the same time
         *
         * @param   pool_size How many components the pool should hold
         */
        template<class T>
        void add_component_pool(const int pool_size)
        {
            _component_pools.add<ComponentPool<T>>(_entity_manager.size(), pool_size);
        }

        template<class T>
        void remove_component_pool()
        {
            _component_pools[typeid(T)].reset();
            _component_pools.erase(typeid(T));
        }

        template<class T>
        void add_system()
        {
            _system_map.add<T>();
        }

        template<class T>
        ObserverPointer<T> get_system()
        {
            return _system_map.get<T>();
        }

        void update(float elapsed_time= 0.0f)
        {
            for (auto& pair : _system_map)
                pair.second->update(*this, 0.0f);
        }

    private:

        PolymorphicMap<SystemInterface> _system_map;
        PolymorphicMap<AbstractPool>    _component_pools;

        EntityPool       _entity_manager;
    };
}
