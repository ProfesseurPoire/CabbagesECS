#pragma once

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
        _count = p._count;
        (*_count)++;
        _ptr = p._ptr;
        _lived = p._lived;
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
            return _ptr;
        return nullptr;
    }

    ~ObserverPointer(){reset();}

    void reset()
    {
        (*_count)--;

        if (*_count == 0)
        {
            delete _lived;
            delete _count;
        }
    }

    int* _count;
    bool* _lived;
    T* _ptr;

protected:

    ObserverPointer(T* ptr, bool* lived)
        :_count(new int(1)), _ptr(ptr), _lived(lived) {}
};
}