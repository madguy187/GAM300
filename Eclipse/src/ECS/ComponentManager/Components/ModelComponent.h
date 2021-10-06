#ifndef MODELCOMPONENT_H
#define MODELCOMPONENT_H

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
#endif /* MODELCOMPONENT_H */