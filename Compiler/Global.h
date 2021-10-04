#pragma once
#include "pch.h"

namespace EclipseCompiler
{
    // Fixing these warnings crashes my engine side. Will look at it soon.
    #pragma warning( disable : 4456 )
    #pragma warning( disable : 4267 )
    #pragma warning( disable : 4244 )
    #pragma warning( disable : 4100 )
    #pragma warning( disable : 4189 )
    #pragma warning( disable : 4458 )

    class Texture
    {
    public:
        Texture() {}

        Texture(std::string path) :
            TexturePath(path)
        {

        }

        Texture(std::string dir, std::string path, aiTextureType type) :
            TextureDirectory(dir),
            TexturePath(path),
            Type(type)
        {

        }

        aiTextureType Type;
        std::string TextureDirectory;
        std::string TexturePath;
    };

    struct Vertex
    {
        glm::vec3 Position{ 0,0,0 };
        glm::vec3 Normal{ 0,0,0 };
        glm::vec2 TextureCoodinates{ 0,0 };
        glm::vec3 Tangents{ 0,0,0 };
        glm::vec4 m_Color{ 0,0,0,0 };
    };

}