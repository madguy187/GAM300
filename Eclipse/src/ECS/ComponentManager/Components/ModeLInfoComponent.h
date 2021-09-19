#pragma once
#include "pch.h"
#include "Library/Math/Vector.h" 
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"

namespace Eclipse
{
    struct ModeLInforComponent
    {
        META_DATA(ModeLInforComponent);
        //TODO
        //ComboIndex if for imgui combolist to select the correct texture type - Tian Yu
        size_t ComboIndex = 0;

        std::string NameOfModel;
        std::string Directory;
        ModelType type = ModelType::MT_UNASSIGNED;
    };
}