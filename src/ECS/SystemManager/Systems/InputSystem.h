#pragma once

#include "../src/ECS/SystemManager/Systems/System/System.h"
#include "InputWrapper.h"
#include "AllInputKeyCodes.h"

namespace Eclipse
{
  class InputSystem : public System
  {
  public:
    void Init();
    void Update() override;

  private:

  InputWrapper test;

    //void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  };
}