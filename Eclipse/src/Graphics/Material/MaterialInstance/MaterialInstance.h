#pragma once

namespace Eclipse
{
    class MaterialInstance
    {
    public:
        std::array<char, 128> MeshName;
        bool HasTexture = false;
        ECVec3 BaseReflectivity = { 0.4f,0.4f,0.4f };

        // Non Textured Parameters
        ECVec3 AlbedoConstant{ 1.0f };
        float AoConstant = 1.0f;
        float MetallicConstant = 1.0f;
        float RoughnessConstant = 1.0f;

        // Textured Parameters
        unsigned int Albedo = 0;
        unsigned int Normal = 0;
        unsigned int Metallic = 0;
        unsigned int Roughness = 0;
        unsigned int Ao = 0;

        // IGNORE PLEASE - For Compiler
        bool Stopper = false;

        MaterialInstance(bool HasTexture = false);
        MaterialInstance(bool HasTextures, const std::string MaterialName);
        MaterialInstance(unsigned albedo_, unsigned normal_, unsigned mettalic_,
            unsigned roughness, unsigned ao, bool HasTexture = false);
    };
}