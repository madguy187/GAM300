#pragma once
#include <array>
#include "AllInputKeyCodes.h"
#include <iostream>

namespace Eclipse
{

  class InputWrapper
  {
  public:
    std::map< std::pair<InputKeycode, int>, std::pair<bool, bool> > KeyContainer;
    using KeyIT = std::map< std::pair<InputKeycode, int>, std::pair<bool, bool> >::iterator;

    bool single;

    struct InputData
    {
      InputState inputState = InputState::Key_NULLSTATE;
      bool SingleFlag = false;
    };

    bool EnablePrint = false;

    void init();
    bool GetIsPrint();
    void SetIsPrint(bool input);

    unsigned char _currPressedCount = 0;					
    unsigned char _lastPressedCount = 0;					

    bool GetKeyTriggered(InputKeycode keycode) ;
    bool GetKeyCurrent(InputKeycode keycode) ;
    bool GetKeyReleased(InputKeycode keycode) ;
    void Update();

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
  };
}