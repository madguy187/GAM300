#pragma once
#include "Graphics/Instancing/DebugBoxes/Box.h"
#include "Graphics/Instancing/DebugSpheres/DebugSphere.h"
#include "Graphics/Instancing/DebugLights/DebugLights.h"
#include "Graphics/Instancing/DebugSpotLights/DebugSpotLights.h"
#include "Graphics/Instancing/DebugBoxes/DebugBoxes.h"

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
        //AABB_ DebugBoxes;
        void ResetInstancedDebugBoxes();
        void AddBoundingRegion(glm::mat4 model, CameraComponent& _camera, Entity EntityID);

    public:
        DebugSphere DebugSpheres;
        DebugLights LightIcons;
        DebugSpotLights SpotLightIcons;
        DebugBoxes DebugBoxes_;
    };
}