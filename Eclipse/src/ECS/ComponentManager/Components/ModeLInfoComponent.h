#pragma once
#include "pch.h"
#include "Library/Math/Vector.h" 

namespace Eclipse
{
    struct ModeLInforComponent
    {
        std::string NameOfModel;
        std::string Directory;
        ModelType type = ModelType::MT_UNASSIGNED;
    };
}