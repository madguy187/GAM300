#pragma once

#include "../System.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/CameraComponent.h"
#include "Graphics.h"
#include "Graphics/OpenGL/OpenGL_Context.h"
#include "Graphics/Camera/CameraManager.h"
#include "Graphics/RendererAPI/GraphicsManager.h"
#include "Graphics/Grid/Grid.h"
#include "Graphics/Instancing/Box.h"
#include "Graphics/Instancing/InstSpheres.h"

namespace Eclipse
{
    class RenderSystem : public System
    {
    public:
        // Empty Instanced Debug Box Container
        static inline AABB_ box;
        static void Init();

        static inline DEBUG_SPHERE Test;

    private:
        void Update() override;
    };
}