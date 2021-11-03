#pragma once

namespace Eclipse
{
    class MaterialInstance
    {
    public:
        std::array<char, 128> Name;
        bool HasTexture = false;
        bool IsNormalMap = false;
        float HeightScale = 0.05f;
        ECVec3 BaseReflectivity = { 0.4f,0.4f,0.4f };
        ECVec3 SurfaceColour = { 1.0f,1.0f,1.0f };

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
        unsigned int Height = 0;

        // IGNORE PLEASE - For Compiler
        bool Stopper = false;

        MaterialInstance(bool HasTexture = false);
        MaterialInstance(bool HasTextures, const std::string MaterialName);
        MaterialInstance(unsigned albedo_, unsigned normal_, unsigned mettalic_,
            unsigned roughness, unsigned ao, bool HasTexture = false);
    };
}