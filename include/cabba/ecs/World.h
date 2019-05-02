#pragma once

#include <cabba/ecs/ComponentPool.h>
#include <cabba/ecs/EntityManager.h>
#include <cabba/ecs/SystemManager.h>

#include <map>
#include <typeindex>

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
         * @brief   Construct the world object and the EntityManager and 
         *          component manager using the given parameters
         */
        World(const int entity_count) 
        :   _entity_manager(*this, entity_count){}

        /*!
         *  @brief Returns a pool of type T. Must have been initialized before
         */
        template<class T>
        ComponentPool<T>* get_pool()
        {
            return static_cast<ComponentPool<T>*>(_pools.at(typeid(T)));
        }

        /*!
         *  @brief  Initialize a pool of type T 
         *  
         *  @param  pool_size Size of the pool
         */
        template<class T>
        void initialize_pool(const int pool_size)
        {
            _pools[typeid(T)] = new ComponentPool<T>(_entity_manager.size(), pool_size);
        }

        template<class T>
        void remove(int entity_id)
        {   // Removing a component pool from the system is just removing
            // it from the list of the registered_components
            _pools[typeid(T)]->registeredComponents.remove(entity_id);
        }

        /*!
         * @brief   Returns a reference to the entity manager
         */
        EntityManager& entity_manager() { return _entity_manager; }

        std::map<std::type_index, AbstractPool*> _pools;

    private:

        EntityManager       _entity_manager;
        SystemManager       _system_manager;
    };
}