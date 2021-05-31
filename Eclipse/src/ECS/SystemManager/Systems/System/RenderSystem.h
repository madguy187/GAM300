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
    static void unLoad();

  private:
    void GlobalRender();
    void Update() override;
    GraphicsManager _GraphicsManager;

    void CheckUniformLoc(Sprite& sprite, unsigned int id);
    void DrawBuffers(unsigned int framebuffer);
    void DrawSecondBuffers(unsigned int framebuffer);
    void test();
  };
}