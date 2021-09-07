#pragma once
#include "Library/Math/Vector.h"

namespace Eclipse
{
    struct MeshComponent3D
    {
        // Store key
        std::string Key;

        unsigned int ID = 0;
        // Set False if model got textures
        bool NoTextures = false; 
        ModelType type = ModelType::MT_UNASSIGNED;
        std::string NameOfModel;
        std::string Directory;
        std::vector<Mesh> Meshes;
        std::vector<Texture> Textures_loaded;
    };
}