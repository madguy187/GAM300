#pragma once

namespace Eclipse
{
    class MaterialInstance
    {
    public:
        std::string Name = "Default";
        bool HasTexture = false;

        // Textured Parameters
        unsigned int Albedo = 0;
        unsigned int Normal = 0;
        unsigned int Metallic = 0;
        unsigned int Roughness = 0;
        unsigned int Ao = 0;

        // Non Textured Parameters
        ECVec3 AlbedoConstant{ 0.0f };
        float AoConstant = 0.0f;
        float MetallicConstant = 0.0f;
        float RoughnessConstant = 0.0f;


        MaterialInstance(bool HasTexture = false);
        MaterialInstance(bool HasTextures, const std::string MaterialName);
        MaterialInstance(unsigned albedo_, unsigned normal_, unsigned mettalic_,
            unsigned roughness, unsigned ao, bool HasTexture = false);
    };
}