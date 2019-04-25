#pragma once

#include <vector>
#include <map>

namespace cabba
{
    class AbstractPool
    {
        public:

        virtual ~AbstractPool(){}
        virtual void unregisterComponent(int id) {};
        virtual void stuff(){}
    };

    template<class T>
    class ComponentPool : public AbstractPool
    {
    public:

        /*
         * @brief   When constructing the Component pool, we need to know
         *          how many entity there is to build our lookup table,
         *          but we can chose to allocate "less" component if we 
         *          know only a few will actually be used...
         *          Maybe I could use something a bit more clever for the 
         *          lookup table? Use a map?
         */
        ComponentPool(int entity_size, int component_size)
        {
            _components = new T[component_size];
            _size       = component_size;
        }

        /*
         *  Returns how many items are currently used from the pool
         */
        int used() const
        {
            return _used;
        }

        /*
         * Returns how many items are left to be used in the pool
         */
        int left()const
        {
            return _size - _used;
        }

        T* Add(int id)
        {
            _used++;
            //registeredComponents.push_back(id);
            return &_components[id];
        }

        /*
         * Remove the component associated to the given entity id
         */
        void remove(int entityId)
        {
            _used--;
        }

        void unregisterComponent(int entityId) override
        {
            /*registeredComponents.erase(
                std::find(registeredComponents.begin(), 
                          registeredComponents.end(), entityId));*/
        }

        T* begin()
        {
            return &_components[0];
        }

        T* end()
        {
            return &_components[_used];
        }

    private:

        /* 
         * The goal of this map is to associate an EntityId with the position
         * of the component inside the pool
         *
         *  EntityId            ComponentPosition
         *      0                       1
         *      1                       5
         *      4                       11
         */
        std::map<int, int>   _entity_lookup_table;

        /*
         * Stores the pool's components
         */
        T * _components;

        /*
         * Stores how many object inside the pool are currently used
         */
        int _used = 0;

        /*
         * Stores the maximum number of item the pool can hold
         */
        int _size;
    };
}