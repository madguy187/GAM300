#pragma once
#include "Graphics/Instancing/Instance_Renderer/Instance_Renderer.h"

namespace Eclipse
{
    class DebugSpotLights : public Instance_Renderer
    {
    public:
        Instance_Renderer DebugSpotLights_;
        std::vector <glm::mat4> instanceMatrix;
        std::vector <Vertex> SpotLightVertices;
        std::vector <unsigned int> SpotLightIndices;

        DebugSpotLights();
        void Init();
        void Addinstance(glm::mat4 TransXRotXScale);
        void ResetInstancedDebugSpotLights();
        void RenderSpotLights();
    };
}