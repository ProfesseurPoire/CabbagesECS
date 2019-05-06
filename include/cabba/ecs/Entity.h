#pragma once

#include <cabba/ecs/World.h>

namespace cabba
{
    using EntityId = int;

    // An Entity is mainly an ID used to group components together
    class Entity
    {
        friend class EntityPool;

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
            return world->get_component_pool<T>()->get(key);
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
                //world->get_component_pool<T>().add(key);
                world->get_component_pool<T>()->add(key);
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
            return world->get_component_pool<T>()->exist(key);
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

        // Use the EntityPool to create a new entity
        Entity();
        // The lifecycle of the entity is handled by the EntityPool
        ~Entity() = default;

        Entity(const Entity&)   = delete;
        Entity(Entity&&)        = delete;

        Entity& operator=(const Entity&)    = delete;
        Entity& operator=(Entity&&)         = delete;

        bool    _to_destroy;
        int     key;

        World* world = nullptr;
        // This is pretty bad actually
        // Also shows that the entity can't manage itself but it is true 
        // though
        // But then that could be used to call entity_pool.something something
        // to actually delete the thing. And so, you can copy Entity
        // object around without a care in the world, and you could reallocate
        // the pool... now there's another obstacle for that -> Components.
        // The components pool would need to be reallocated as well with that 
        // design, which would make the operation VERY expensive and not really
        // welcomed at all. I still think it's better to goes with the idea
        // that you can only have X entities, and if you use more, you should
        // reallocate the everything right from the start.
        EntityPool* _entity_manager;
    };
}