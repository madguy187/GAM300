#pragma once
#include "Library/Math/Vector.h"

namespace Eclipse
{
    struct MeshComponent
    {
        //--------------------------------------
        // 3D Models
        //--------------------------------------
        // Store key
        std::string Key;

        std::string MeshName;
        unsigned int VBO;
        unsigned int VAO;
        unsigned int EBO;
        bool NoTex;
        aiColor4D Diffuse;
        aiColor4D Specular;
        aiColor4D Ambient;
        std::vector<Vertex> Vertices;
        std::vector<unsigned int> Indices;

        //---------------------------------------
        // Anything Else Other than Assimp Models
        //---------------------------------------
        ECVec3 color{ 1.0f, 1.0f, 1.0f };
        ECVec2 textureIdx{ 0.0, 0.0 };
        bool isQuad = false;
        glm::mat4 modelNDC_xform{ 0 };
        std::string modelRef;
        std::string shaderRef;
        //std::string textureRef;
        unsigned int ID = 0;
        std::string name;
        unsigned int newLayer{ 0 };
        unsigned int layerNum{ 0 };
        float transparency{ 1.0f };
        ECVec4 lightColor{ 1.0f,1.0f,1.0f,1.0f };
    };
}