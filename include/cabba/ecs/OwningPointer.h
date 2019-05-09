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

// Static Functions

    template<typename... Args>
    OwningPointer make(Args&&... args)
    {
        return OwningPointer(new T(std::forward<Args>(args)...));
    }

// Lifecycle

    OwningPointer() = default;
    OwningPointer(T* t) { _ptr = t; }

    // Can't copy a Owning pointer around
    OwningPointer(const OwningPointer&) = delete;
    OwningPointer(OwningPointer&& p) { move(p); }

    ~OwningPointer() { reset(); }

// Asignment Operator

    OwningPointer& operator=(const OwningPointer&) = delete;
    OwningPointer& operator=(OwningPointer&& p)noexcept
    {
        move(p);
        return *this;
    }

// Operators

    T* operator->() { return _ptr;  }
    T& operator*()  { return *_ptr; }
    operator bool() { return _ptr != nullptr; }

// Functions
    
    void reset()
    {
        if (_lived != nullptr)
            * _lived = false;

        delete _ptr;
        _ptr = nullptr;
    }

    ObserverPointer<T> create_observer()
    {
        // Creates a thing in the heap to track if the 
        // pointer is still valid or not
        _lived = new bool(true);
        return ObserverPointer<T>(_ptr, _lived);
    }

    template<class U>
    ObserverPointer<U> create_derived_observer();

private:

// Functions

    void move(OwningPointer & p)
    {
        _ptr        = p._ptr;
        _lived      = p._lived;
        p._ptr      = nullptr;
        p._lived    = nullptr;
    }

// Member variables 

    // When the owning pointer creates an observer, it also creates a place
    // in the heap where he stores the current state of the owned data.
    // If the data is destroyed, _lived goes to false, and the Observer
    // are awake that the data no longer lives on
    bool*   _lived  = nullptr;  
                                
    // Pointer to the data currently managed by the owning pointer
    T*      _ptr    = nullptr;
};

template<class T>
template<class U>
ObserverPointer<U> OwningPointer<T>::create_derived_observer()
{
    static_assert(std::is_base_of<T, U>(),
                  "Template parameter is not a base of observed Pointer");
    // Creates a bool on the heap so the observer knows
    // if the pointer is still valid or not
    _lived = new bool(true);
    return ObserverPointer<U>(static_cast<U*>(_ptr), _lived);
}

}