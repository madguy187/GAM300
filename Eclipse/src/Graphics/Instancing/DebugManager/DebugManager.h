#pragma once
#include "Graphics/Instancing/Box.h"

namespace Eclipse
{
    enum class DEBUG_SHAPES
    {
        DS_NONE = 0,
        DS_BOX = 1,
        DS_SPHERE = 2,
        MAXCOUNT
    };

    class DebugManager
    {
    public:
        DebugManager();
        void Init();

        void Reset();

    public:
        AABB_ DebugBoxes;
        void ResetInstancedDebugBoxes();
        void Render();

    public:
        DEBUG_SPHERE DebugSpheres;
        std::vector <glm::mat4> instanceMatrix;
        std::vector <Vertex> SphereVertices;
        std::vector <unsigned int> SphereIndices;
        void Addinstance(glm::mat4 TransXRotXScale);
        void ResetInstancedDebugSpheres();
        void PrepareData();
        void RenderBoundingSpheres();
    };
}