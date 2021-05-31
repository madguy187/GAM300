#pragma once

#include "System.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "ECS/ComponentManager/Components/CameraComponent.h"
#include "Graphics.h"
#include "Graphics/InputHandler/InputWrapper.h"
#include "Graphics/OpenGL/OpenGL_Context.h"
#include "Graphics/Camera/CameraManager.h"
#include "Graphics/RendererAPI/GraphicsManager.h"

namespace Eclipse
{
  class RenderSystem : public System
  {
  public:
    static void Init();
    static Signature RegisterAll();

  private:
    void Update() override;
  };
}