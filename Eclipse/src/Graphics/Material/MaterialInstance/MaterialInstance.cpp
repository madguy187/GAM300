#include "pch.h"
#include "Graphics/Material/MaterialInstance/MaterialInstance.h"

namespace Eclipse
{
    // No Texture , so we will play with colours
    MaterialInstance::MaterialInstance(bool HasTexture)
    {

    }

    // Got Tetxures , so we will load textures
    MaterialInstance::MaterialInstance(bool HasTextures_, const std::string MaterialName):
        HasTexture(HasTextures_),
        Name(MaterialName)
    {

    }

    // No Texture , so we will play with colours
    MaterialInstance::MaterialInstance(unsigned albedo_, unsigned normal_, unsigned mettalic_, unsigned roughness, unsigned ao, bool HasTexture) :
        Albedo(albedo_),
        Normal(normal_),
        Metallic(mettalic_),
        Roughness(roughness),
        Ao(ao)
    {

    }
}
