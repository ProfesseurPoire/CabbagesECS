#pragma once

#include <cabba/String.h>
#include <cabba/containers/List.h>

#include <cabba/ecs/World.h>

#include <vector>
#include <typeindex>
#include <typeinfo>

namespace cabba
{
    using EntityId = int;

    // An Entity is mainly an ID used to group components together
    class Entity
    {
        friend class EntityManager;

    public:

        bool enabled;

        String name;
        String type;

        List<String> tags;

        EntityId id()const;

        /*
         * @brief Retrieve the component of type T associated with the entity
         *  
         *  Returns nullptr if the component has not been registered beforehand
         *
         *  Warning : this function might be slow, you'll probably be 
         *  better off using systems
         */
        template<class T>
        T* GetComponent()
        {
            for (auto& typeIndex : _registeredComponents)
            {
                if (typeIndex == typeid(T))
                {
                    return &(*world->getPool<T>()).components[id()];
                }
            }
            return nullptr;
        }

        template<class T>
        T* getComponent()
        {
            
        }

        /*
         *  @brief Attach a new component to the entity
         *
         *  Only 1 component of type T can exist at any given time
         *  so if the component was already attached, we return it 
         */
        template<class T>
        T* AddComponent()
        {
            if (hasComponent<T>())
            {
                return GetComponent<T>();
            }
            _registeredComponents.push_back(typeid(T));
            return world->getPool<T>()->Add(id());
        }

        template<class T>
        bool hasComponent()
        {
            std::type_index typeIndex = typeid(T);

            for (const std::type_index& component : _registeredComponents)
            {
                if (typeIndex == component)
                {
                    return true;
                }
            }
            return false;
        }

        void destroy()
        {
            _to_destroy = true;
        }

        const std::vector<std::type_index> registeredComponents()
        {
            return _registeredComponents;
        }

    protected:

        // Use the EntityManager to create a new entity
        Entity();

        bool    _to_destroy;
        int     key;

        std::vector<std::type_index> _registeredComponents;

        World* world = nullptr;
    };
}