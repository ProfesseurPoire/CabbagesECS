#pragma once

#include <cabba/ecs/ObserverPointer.h>
#include <type_traits>

namespace cabba
{

/*
 * @brief   Owns a pointer. The difference with std::unique_ptr is the
 *          observer mechanism. You can create Observers that points
 *          to this owningPointer. If the OwningPointer is deleted,
 *          the observers knows about this
 */
template<class T>
class OwningPointer
{
public:

    OwningPointer() = default;

    template<typename T, typename... Args>
    OwningPointer(Args&&... args)
    {
        _ptr = new T(std::forward<Args>(args)...);
    }

    OwningPointer(T* t) { _ptr = t; }
    OwningPointer(OwningPointer&& p) { move(p); }

    // Can't copy a Owning pointer around
    OwningPointer(const OwningPointer&) = delete;
    OwningPointer& operator=(const OwningPointer&) = delete;

    OwningPointer& operator=(OwningPointer&& p)noexcept
    {
        move(p);
        return *this;
    }

    ~OwningPointer()    { reset();                  }
    T* operator->()     { return _ptr;              }
    T& operator*()      { return *_ptr;             }
    operator bool()     { return _ptr != nullptr;   }

    void reset()
    {
        if (_lived != nullptr)
            * _lived = false;
        delete _ptr;
    }

    template<class U>
    ObserverPointer<U> create_observer()
    {
        static_assert(std::is_base_of<T, U>(),
                      "Template parameter is not a base of observed Pointer");
        // Creates a thing in the heap to track if the 
        // pointer is still valid or not
        _lived = new bool(true);
        return ObserverPointer<U>(static_cast<U*>(_ptr), _lived);
    }

    bool*   _lived  = nullptr;
    T*      _ptr    = nullptr;

private:

    void move(OwningPointer & p)
    {
        _ptr        = p._ptr;
        _lived      = p._lived;
        p._ptr      = nullptr;
        p._lived    = nullptr;
    }
};
}