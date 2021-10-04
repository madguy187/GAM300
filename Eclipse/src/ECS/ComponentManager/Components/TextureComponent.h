#ifndef TEXTURECOMPONENT_H
#define TEXTURECOMPONENT_H
#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"

namespace Eclipse
{
    struct TextureComponent
    {
        META_DATA(TextureComponent);
        std::string textureRef;
    };
}

#endif /* TEXTURECOMPONENT_H */
