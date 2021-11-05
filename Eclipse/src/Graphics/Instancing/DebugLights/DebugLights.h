#pragma once
#include "Graphics/Instancing/Instance_Renderer/Instance_Renderer.h"

namespace Eclipse
{
    class DebugLights : public Instance_Renderer
    {
    public:
        Instance_Renderer DebugLights_;
        std::vector <glm::mat4> instanceMatrix;
        std::vector <Vertex> LightVertices;
        std::vector <unsigned int> LightIndices;

        DebugLights();
        void Init();
        void Addinstance(glm::mat4 TransXRotXScale);
        void ResetInstancedDebugLights();
        void RenderLights();
    };
}