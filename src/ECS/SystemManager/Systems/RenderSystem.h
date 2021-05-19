#pragma once

#include "../src/ECS/SystemManager/Systems/System/System.h"
#include "ECS/ComponentManager/Components/TransformComponent.h"
#include "Graphics.h"
#include <glm/gtc/type_ptr.hpp> // for glm::value_ptr
#include "glm/gtx/matrix_transform_2d.hpp"
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