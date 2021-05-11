#pragma once

#include "pch.h"

// binds a data type to an index
template <typename U>
class TypeID
{
    // initialise counter in whoever owns it
    // e.g. unsigned TypeID<int>::_counter = 0;
    static unsigned _counter;

public:
    template <typename T>
    static void RegisterID()
    {
        RemoveConstRef< std::decay_t<T> >();
    }

    template <typename T>
    static unsigned GetID()
    {
        return RemoveConstRef< std::decay_t<T> >();
    }

    template <typename T>
    static unsigned RemoveConstRef()
    {
        static unsigned const id = _counter++;
        return id;
    }

    static unsigned GetSize()
    {
        return _counter;
    }
};
