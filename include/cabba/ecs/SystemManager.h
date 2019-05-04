#pragma once

#include <cabba/containers/List.h>
#include <map>
#include <type_traits>
#include <typeindex>

namespace cabba
{
    struct SystemInterface;
    class World;

    class SystemManager
    {
    public:

        friend class World;

    // Lifecycle

        SystemManager(const SystemManager&) = delete;
        SystemManager(SystemManager&&)      = delete;
        ~SystemManager();

    // Assignment Operators

        SystemManager& operator=(const SystemManager&)  = delete;
        SystemManager& operator=(SystemManager&&)       = delete;
    // Methods

        /*!
         * @brief   Checks if the system contains a system of the given Type
         */
        template<class T>
        bool has() const { return _systems.count(typeid(T))>0; }

        template<class T>
        void add()
        {
            static_assert(std::is_base_of<SystemInterface, T>(), "T must inherit from SystemInterface");
            static_assert(std::is_default_constructible<T>(),    "T must have a default constructor");

            if (!has<T>())
            {
                _systems.emplace(typeid(T), new T);
            }
        }

        template<class T>
        void remove()
        {
            static_assert(std::is_base_of<SystemInterface, T>(), "T must inherit from SystemInterface");
            _systems.erase(typeid(T));
        }

        template<class T>
        T& get()
        {
            static_assert(std::is_base_of<SystemInterface, T>(), "T must inherit from SystemInterface");

            if (has<T>())
            {
                return *static_cast<T*>(_systems[typeid(T)]);
            }
        }

        /*!
         * @brief   Returns how many System have been registered 
         */
        int size()const;

        /*!
         * @brief   Delete every system that has been added to the manager
         */
        void clear();

        /*
         * @brief   Returns true if the manager doesn't store any system
         */
        bool empty()const;

        // Update every System and lambda registered inside the Manager
        void update(World& world, float elapsedTime);

    private:

        SystemManager() = default;

        // Temporary, need to use my own mappy thingie
        std::map<std::type_index, SystemInterface*> _systems;
    };
};