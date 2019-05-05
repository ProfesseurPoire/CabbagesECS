#pragma once

#include <cabba/ecs/EntityManager.h>
#include <cabba/ecs/SystemManager.h>
#include <cabba/ecs/ComponentPool.h>

#include <memory>
#include <iostream>

namespace cabba
{
  /*  template<class T>
    class ObserverPointer
    {
    public:

        friend class OwningPointer;

        bool valid()const
        {
            return *_lived;
        }

        ObserverPointer(const ObserverPointer p)
        {
            *_count++;
            _ptr    = p._ptr;
            _lived  = p._lived;
        }

        ~ObserverPointer()
        {
            *_count--;
            if(*_count==0)
            {
                delete _lived;
                delete _count;
            }
        }

        int* _count;
        bool* _lived;
        T* _ptr;

    protected:

        ObserverPointer()
        {

        }
    };

    template<class T>
    class OwningPointer
    {
    public:

        OwningPointer(T * t)
        {
            _ptr = t;
        }

        ~OwningPointer()
        {
            if(_lived!=nullptr)
                *_lived = false;

            delete _ptr;
        }

        ObserverPointer<T> create_observer()
        {
            _lived = new bool(true);

            ObserverPointer<T> observer;
            observer._ptr   = _ptr;
            observer._count = new int(1);
            observer._lived = _lived;

            return observer;
        }

        bool*   _lived = nullptr;
        T*      _ptr;
    };*/

    //
    //// Making my own clunck Pointer because SharedPtr and 
    //// UniquePtr doesn't works 
    //template<class T>
    //class CustomPointer
    //{
    //    List<CustomPointer*>* _other_pointers = nullptr;

    //    CustomPointer(T* v)
    //    {
    //        // This is the original
    //        _ptr = v;
    //        _other_pointers = new List<CustomPointer>();
    //        _other_pointers->add(this);
    //    }

    //    CustomPointer(const CustomPointer& c)
    //    {
    //        c._other_pointers.add(this);
    //        _other_pointers.add(&c);
    //        _ptr = c._ptr;
    //    }

    //    ~CustomPointer()
    //    {
    //        
    //    }

    //    void reset()
    //    {
    //        _ptr = nullptr;
    //        for(auto : _other_pointers)
    //        {
    //            
    //        }
    //    }

    //    T* _ptr;

    //    
    //};
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
        std::unique_ptr<ComponentPool<T>>& get_component_pool()
        {
            return static_cast<std::unique_ptr<ComponentPool<T>>>(_component_pools.at(typeid(T)));
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
            _component_pools[typeid(T)] = std::unique_ptr<ComponentPool<T>>(
                _entity_manager.size(), pool_size);
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

        std::map<std::type_index, std::unique_ptr<AbstractPool>>    _component_pools;

        std::map<std::type_index, SystemInterface*> _systems;

        EntityPool       _entity_manager;
        SystemMap       _system_manager;
    };
}
