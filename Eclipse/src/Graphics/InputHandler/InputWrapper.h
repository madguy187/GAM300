#pragma once
#include "AllInputKeyCodes.h"
#include <map>

namespace Eclipse
{
  typedef std::map< std::pair<InputKeycode, int>, std::pair<bool, bool> > InputKeyContainer;
  using KeyIT = std::map< std::pair<InputKeycode, int>, std::pair<bool, bool> >::iterator;

  class InputWrapper
  {
  private:
    InputKeyContainer KeyContainer;
    InputKeyContainer HoldKeyContainer;

    bool single;

    struct InputData
    {
      InputState inputState = InputState::Key_NULLSTATE;
      bool SingleFlag = false;
    };

    bool EnablePrint = false;

    unsigned char _currPressedCount = 0;
    unsigned char _lastPressedCount = 0;

    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

  public:
    void init();

    bool GetKeyTriggered(InputKeycode keycode);
    bool GetKeyCurrent(InputKeycode keycode);
    bool GetKeyReleased(InputKeycode keycode);

    void Update();
    bool GetIsPrint();
    void SetIsPrint(bool input);
  };
}