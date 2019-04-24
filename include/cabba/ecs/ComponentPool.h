#pragma once

#include <vector>

namespace cabba
{
    class AbstractPool
    {
        public:

        virtual ~AbstractPool(){}
        virtual void unregisterComponent(int id) {};
        virtual void stuff(){}
    };

    template<class T, int Size = 10000>
    class ComponentPool : public AbstractPool
    {
    public:

        /*
         * @brief   When construction the Component pool, we need to know
         *          how many entity there is to build our lookup table,
         *          but we can chose to allocate "less" component if we 
         *          know only a few will actually be used...
         *          Maybe I could use something a bit more clever for the 
         *          lookup table? Use a map?
         */
        ComponentPool(int entity_size, int component_size)
        {
            
        }

        /*
         *  Returns how many items are still available in the pool
         */
        int left() const
        {
            return Size - registeredComponents.size();
        }

        T* Add(int id)
        {
            registeredComponents.push_back(id);
            return &components[id];
        }

        void unregisterComponent(int entityId) override
        {
            registeredComponents.erase(
                std::find(registeredComponents.begin(), 
                          registeredComponents.end(), entityId));
        }

        T components[Size]{};
        std::vector<int>	registeredComponents;
    };
}