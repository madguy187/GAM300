#pragma once
#include "pch.h"
namespace EclipseCompiler
{
    class IBinary
    {
    public:
        std::vector<size_t> OffSetsForObject;
        virtual void Calculate() = 0;
    };
}