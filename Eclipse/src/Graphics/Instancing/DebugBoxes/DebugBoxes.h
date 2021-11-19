#pragma once
#include "Graphics/Instancing/Instance_Renderer/Instance_Renderer.h"

namespace Eclipse
{
    class DebugBoxes : public Instance_Renderer
    {
    public:
        Instance_Renderer DebugBoxes_;
        std::vector <glm::mat4> instanceMatrix;
        std::vector <Vertex> DebugBoxesVertices;
        std::vector <unsigned int> DebugBoxesIndices;

        DebugBoxes();
        void Init();
        void Addinstance(glm::mat4 TransXRotXScale);
        void ResetInstancedDebugBoxes();
        void RenderDebugBoxes();
    };
}