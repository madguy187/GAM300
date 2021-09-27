#pragma once
#include "pch.h"

class Texture
{
public:
    Texture() {}

    Texture(std::string dir, std::string path, aiTextureType type) :
        TextureDirectory(dir),
        TexturePath(path),
        Type(type)
    {

    }

    aiTextureType Type;
    std::string TexturePath;
    std::string TextureDirectory;
};

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TextureCoodinates;
    glm::vec4 m_Color;
};
