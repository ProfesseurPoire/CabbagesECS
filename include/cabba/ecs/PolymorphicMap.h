#pragma once

#include <map>
#include <type_traits>
#include <typeindex>

#include <cabba/ecs/OwningPointer.h>

namespace cabba
{

/*!
 * @brief   Map that associates the type of a class that 
 *          inherits from the given template argument with a pointer
 *          to an instance of that type
 *
 *          There can only be 1 instance for a given type
 */
template<class Base>
class PolymorphicMap
{
public:

    // typename helps the template to know whether iterator
    // will be a typename or a member. He can't know that at compile time
    // apparently
    using Iterator = typename std::map<std::type_index, OwningPointer<Base>>::iterator;

// Lifecycle

    PolymorphicMap() = default;

    PolymorphicMap(const PolymorphicMap&)               = delete;
    PolymorphicMap(PolymorphicMap&&)                    = delete;

    PolymorphicMap& operator=(PolymorphicMap&)          = delete;
    PolymorphicMap& operator=(const PolymorphicMap&)    = delete;

// Methods

    Iterator begin(){ return _map.begin();}
    Iterator end()  { return _map.end();}

    template<typename Derived, typename... Args>
    void emplace(Args&& ... args)
    {
        if (!has<Derived>())
            _map.emplace(typeid(Derived), new Derived(std::forward<Args>(args)...));
    }

    template<class Derived>
    void add()
    {
        static_assert(std::is_base_of<Base, Derived>(), "Template argument must inherit from SystemInterface");
        static_assert(std::is_default_constructible<Derived>(), "Template argument must have a default constructor");

        if (!has<Derived>())
        {
            _map.emplace(typeid(Derived),  new Derived);
        }
    }

    template<typename Derived, typename... Args>
    void add(Args&& ... args)
    {
        static_assert(std::is_base_of<Base, Derived>(), "Template argument must inherit from SystemInterface");

        if (!has<Derived>())
        {
            _map.emplace(typeid(Derived), new Derived(std::forward<Args>(args)...));
        }
    }

    template<class Derived>
    bool has()
    {
        static_assert(std::is_base_of<Base, Derived>(), "T must inherit from SystemInterface");
        return _map.count(typeid(Derived)) > 0;
    }

    template<class Derived>
    void remove()
    {
        static_assert(std::is_base_of<Base, Derived>(), "T must inherit from SystemInterface");
        _map.erase(typeid(Derived));
    }

    template<class Derived>
    ObserverPointer<Derived> get()
    {
        static_assert(std::is_base_of<Base, Derived>(), "T must inherit from SystemInterface");

        if (has<Derived>())
            return _map[typeid(Derived)].create_observer<Derived>();
    }

    /*!
     * @brief   Returns how many System have been registered
     */
    int size()const { return _map.size(); }

    /*!
     * @brief   Delete every system that has been added to the manager
     */
    void clear() 
    {
        for (auto& pair : _map)
            system.second.reset();
        _map.clear();
    }

    /*
     * @brief   Returns true if the manager doesn't store any system
     */
    bool empty()const{ return _systems.size() == 0; }

private:

    std::map<std::type_index, OwningPointer<Base>> _map;
};
}