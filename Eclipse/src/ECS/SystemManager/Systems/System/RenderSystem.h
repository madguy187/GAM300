#pragma once

#include "System.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "Graphics.h"
#include "InputWrapper.h"

namespace Eclipse
{
  class RenderSystem : public System
  {
  public:
    InputWrapper InputHandler;

    static void Load();
    void Init();
    void Render();
    void Update() override;
    static void unLoad();

  private:
    void CheckUniformLoc(Sprite& sprite);
    void DrawBuffers(unsigned int framebuffer);
    void DrawSecondBuffers(unsigned int framebuffer);
  };
}