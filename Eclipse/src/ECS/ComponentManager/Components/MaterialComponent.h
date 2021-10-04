#ifndef materialCOMPONENT_H
#define materialCOMPONENT_H

#include "Reflection/Meta.h"
#include "Reflection/RemTypeQual.h"

namespace Eclipse
{
    struct MaterialComponent
    {
        META_DATA(MaterialComponent);
        
        MaterialModelType Modeltype = MaterialModelType::MT_NONE;
        ECVec3 ambient{ 0.0f,0.0f,0.0f };
        ECVec3 diffuse{ 1.0f,1.0f,1.0f };
        ECVec3 specular{ 1.0f,1.0f,1.0f };
        bool Highlight = false;
        bool NoTextures = true;
        float shininess = 0.25f;
        float MaximumShininess = 128.0f;

        //ComboIndex if for imgui combolist to select the correct model type - Tian Yu
        size_t ComboIndex;

        /*************************************************************************
          Components for a Texture
        *************************************************************************/
        //ComboIndex if for imgui combolist to select the correct model type - Tian Yu
        size_t TextureComboIndex;
        TextureType Type = TextureType::TT_UNASSIGNED;

        // Basic Meshes
        bool hasTexture = false;
        std::string TextureRef;

        // Models
        std::string TextureKey;
        std::vector<Texture> HoldingTextures;
    };
}
#endif /* materialCOMPONENT_H */