#pragma once

#include <cabba/ecs/EntityManager.h>
#include <cabba/ecs/SystemManager.h>
#include <cabba/ecs/ComponentPool.h>

#include <memory>
#include <iostream>
#include <type_traits>

namespace cabba
{
    template<class T>
    class ObserverPointer
    {
    public:

        template <class T>
        friend class OwningPointer;

        bool valid()const
        {
            return *_lived;
        }

        ObserverPointer(const ObserverPointer& p)
        {
            _count  = p._count;
            (*_count)++;
            _ptr    = p._ptr;
            _lived  = p._lived;
        }

        ObserverPointer& operator=(const ObserverPointer& p)
        {
            _count = p._count;
            (*_count)++;
            _ptr = p._ptr;
            _lived = p._lived;
        }

        T* operator->()
        {
            if (*_lived)
            {
                return _ptr;
            }
            return nullptr;
        }

        ~ObserverPointer()
        {
            (*_count)--;

            if(*_count==0)
            {
                delete _lived;
                delete _count;
            }
        }

        int*    _count;
        bool*   _lived;
        T*      _ptr;

    protected:

        ObserverPointer(T* ptr, bool *lived) 
            :_count(new int(1)), _ptr(ptr), _lived(lived){}
    };

    template<class T>
    class OwningPointer
    {
    public:

        OwningPointer() = default;

       /* template<typename T, typename... Args>
        OwningPointer(Args&&... args)
        {
            return unique_ptr<T>(new T(std::forward<Args>(args)...));
        }*/

        OwningPointer(T*  t)
        {
            _ptr = t;
        }

        // Can't copy a Owning pointer around
        OwningPointer(const OwningPointer&) = delete;
        OwningPointer& operator=(const OwningPointer&) = delete;

        OwningPointer(OwningPointer&& p)
        {
            move(p);
        }

        OwningPointer& operator=(OwningPointer&& p)
        {
            move(p);
            return *this;
        }

        ~OwningPointer()
        {
            reset();
        }

        T* operator->()
        {
            return _ptr;
        }

        T& operator*()
        {
            return *_ptr;
        }

        operator bool()
        {
            return _ptr != nullptr;
        }

        void reset()
        {
            if(_lived!=nullptr)
                *_lived = false;

            delete _ptr;
        }

        template<class U>
        ObserverPointer<U> create_observer()
        {
            static_assert(std::is_base_of<T, U>(), 
                          "Template parameter is not base of observed Pointer");

            // Creates a thing in the heap to track if the 
            // pointer is still valid or not
            _lived = new bool(true);
            return ObserverPointer<U>(static_cast<U*>(_ptr), _lived);
        }

        bool*   _lived = nullptr;
        T*      _ptr;

    private:

        void move(OwningPointer& p)
        {
            _ptr        = p._ptr;
            _lived      = p._lived;
            p._ptr      = nullptr;
            p._lived    = nullptr;
        }
    };

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

        /*!
         * @brief   Returns a reference to the system manager
         */
        SystemMap& system_manager() { return _system_manager; }

        void release_entity_immediate(Entity* e);

        /*!
         * @brief   Returns a pool of type T. Must have been initialized before
         */
        template<class T>
        ObserverPointer<ComponentPool<T>> get_component_pool()
        {
            return _component_pools.at(typeid(T)).create_observer<ComponentPool<T>>();
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
            _component_pools[typeid(T)] = std::move( OwningPointer<AbstractPool>(
                    new ComponentPool<T>(
                        _entity_manager.size(), pool_size
                        )
                    
                    ));
        }

        template<class T>
        void remove_component_pool()
        {
            _component_pools[typeid(T)].reset();
            _component_pools.erase(typeid(T));
        }

        // So, I don't really like to call that "create" because it doesn't
        // really convey the idea that we're actually pooling an entity
       /* Entity& get_unused_entity();
        Entity& get_new_entity();
        Entity& get_entity();

        entity_pool.get();*/

        /*Entity& acquire_entity();
        void release_entity(Entity& e);*/
/*
        template<class T>
        SystemInterface* create_system()
        {

        }*/

    private:

        std::map<std::type_index, OwningPointer<AbstractPool>>    _component_pools;
        std::map<std::type_index, SystemInterface*> _systems;

        EntityPool       _entity_manager;
        SystemMap       _system_manager;
    };
}
