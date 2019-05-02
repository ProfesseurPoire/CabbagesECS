#pragma once

#include <cabba/ecs/World.h>

namespace cabba
{
    using EntityId = int;

    // An Entity is mainly an ID used to group components together
    class Entity
    {
        friend class EntityManager;

    public:

        bool enabled;

        EntityId id()const;

        /*!
         * @brief Retrieve the component of type T associated with the entity
         *  
         *  Returns nullptr if the component has not been registered beforehand
         *
         *  Warning : this function might be slow, you'll probably be 
         *  better off using systems
         */
        template<class T>
        typename ComponentPool<T>::Handle get_component()
        {
            return world->get_pool<T>()->get(key);
        }

        /*!
         * @brief   Attach a new component to the entity
         *          Only 1 component of type T can exist at any given time
         *          so if the component was already attached, we return it 
         */
        template<class T>
        void add_component()
        {
            if (!has_component<T>())
            {
                world->get_pool<T>()->add(key);
            }
        }

        /*!
         * @brief   Checks if the entity is associated to a component of type T
         * 
         * @return  Returns true if a component has been associated, false
         *          otherwise
         */
        template<class T>
        bool has_component()
        {
            // I don't know if I should also check if the pool exist
            // maybe adds an assert?
            return world->get_pool<T>()->exist(key);
        }

        /*!
         * @brief   Marks the entity for destruction
         * 
         *          You don't want to directly destroy entities when you're
         *          still stuck inside the game loop (messes up indices and 
         *          iteration, can turn even worse if some allocation happens 
         *          if you're using iterators), so we delay their destruction 
         *          at a later stage by setting this flag to true
         */
        void destroy()
        {
            _to_destroy = true;
        }

    protected:

        // Use the EntityManager to create a new entity
        Entity();

        bool    _to_destroy;
        int     key;

        World* world = nullptr;
    };
}