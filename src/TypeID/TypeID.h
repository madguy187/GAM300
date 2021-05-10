#pragma once

#include "pch.h"

template <typename U>
class TypeID
{
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
        static unsigned id = _counter++;
        return id;
    }
};
