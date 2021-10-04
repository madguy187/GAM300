#pragma once
#include "pch.h"
#include "Library/Math/Vector.h" 
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"

namespace Eclipse
{
    struct ModelComponent
    {
        META_DATA(ModelComponent);
        
        std::string NameOfModel;
        std::string Directory;
        ModelType type = ModelType::MT_UNASSIGNED;

        //TODO
        //ComboIndex if for imgui combolist to select the correct texture type - Tian Yu
        size_t ComboIndex = 0;

    };
}