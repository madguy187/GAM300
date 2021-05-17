#include "pch.h"
#include "InputSystem.h"

using namespace Eclipse;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
  InputKeycode c = static_cast<InputKeycode>(action);

  InputWrapper Test;

  switch (action)
  {
  case GLFW_RELEASE:
    Test.GetKeyPressed(c);
    break;

  case GLFW_PRESS:
    Test.GetKeyPressed(c);
    break;
  }
}

void Eclipse::InputSystem::Init()
{
  test.init();
}


void Eclipse::InputSystem::Update()
{
 test.GetKeyPressed(InputKeycode::KeyCode_0);
 test.GetKeyReleased(InputKeycode::KeyCode_0);
 test.GetKeyPressed(InputKeycode::KeyCode_9);
 test.GetKeyReleased(InputKeycode::KeyCode_9);

  //glfwSetKeyCallback(GLHelper::ptr_window, key_callback);
}
