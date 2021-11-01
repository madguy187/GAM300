#pragma once
#include "Graphics/Instancing/Box.h"
#include "Graphics/Instancing/Instance_Renderer.h"

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
        bool Visible = false;

        DebugManager();
        void Init();
        void Reset();
        void Render();

    public:
        AABB_ DebugBoxes;
        void ResetInstancedDebugBoxes();
        void AddBoundingRegion(glm::mat4 model, CameraComponent& _camera, Entity EntityID);

    public:
        Instance_Renderer DebugSpheres;
        std::vector <glm::mat4> instanceMatrix;
        std::vector <Vertex> SphereVertices;
        std::vector <unsigned int> SphereIndices;
        void Addinstance(glm::mat4 TransXRotXScale);
        void ResetInstancedDebugSpheres();
        void PrepareData();
        void RenderBoundingSpheres();
    };
}