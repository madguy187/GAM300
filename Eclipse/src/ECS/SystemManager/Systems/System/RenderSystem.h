#pragma once

#include "System.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "Graphics.h"
#include "Graphics/InputHandler/InputWrapper.h"
#include "Graphics/OpenGL/OpenGL_Context.h"

namespace Eclipse
{
  class RenderSystem : public System
  {
  public:

    static void Load();
    void Init();
    void Render();
    void Update() override;
    static void unLoad();

  private:
    InputWrapper InputHandler;
    static OpenGL_Context mRenderContext;

    void CheckUniformLoc(Sprite& sprite);
    void DrawBuffers(unsigned int framebuffer);
    void DrawSecondBuffers(unsigned int framebuffer);
  };
}