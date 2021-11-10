#pragma once

#include "../System.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/CameraComponent.h"
#include "Graphics.h"
#include "Graphics/OpenGL/OpenGL_Context.h"
#include "Graphics/Camera/CameraManager.h"
#include "Graphics/RendererAPI/GraphicsManager.h"
#include "Graphics/Grid/Grid.h"

#include "Graphics/RendererAPI/RenderManager/RenderManger.h"

namespace Eclipse
{
    class RenderSystem : public System
    {
        RenderManager Renderer;
    public:
        static void Init();
    private:
        void Update() override;
    };
}