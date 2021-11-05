#pragma once
#include "Graphics/Instancing/Instance_Renderer/Instance_Renderer.h"

namespace Eclipse
{
    class DebugSphere : public Instance_Renderer
    {
    public:
        Instance_Renderer DebugSpheres;
        std::vector <glm::mat4> instanceMatrix;
        std::vector <Vertex> SphereVertices;
        std::vector <unsigned int> SphereIndices;

        DebugSphere();
        void Init();
        void Addinstance(glm::mat4 TransXRotXScale);
        void ResetInstancedDebugSpheres();
        void PrepareData();
        void RenderBoundingSpheres();
    };
}