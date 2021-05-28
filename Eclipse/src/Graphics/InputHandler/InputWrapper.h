#pragma once
#include "AllInputKeyCodes.h"
#include <map>

namespace Eclipse
{
  typedef std::map< std::pair<InputKeycode, int>, std::pair<bool, InputState> > InputKeyContainer;
  using KeyIT = std::map< std::pair<InputKeycode, int>, std::pair<bool, InputState> >::iterator;

  class InputWrapper
  {
  private:
    InputKeyContainer KeyContainer;
    InputKeyContainer HoldKeyContainer;

    bool EnablePrint = false;
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

  public:

    InputWrapper();
    bool GetKeyTriggered(InputKeycode keycode);

    // hi
    bool GetKeyCurrent(InputKeycode keycode);
    bool GetIsPrint();

  private:
    void init();
    void Update();
    void SetIsPrint(bool input);
    bool GetKeyReleased(InputKeycode keycode);
    bool GetHoldKeyReleased(InputKeycode keycode);
    void RegisterTriggerInput(InputKeycode keycode, int keypressFlag, bool flag, InputState input);
    void RegisterHoldInput(InputKeycode keycode, int keypressFlag, bool flag, InputState input);
    bool IsKeyPressed(int input);
    bool IsKeyReleased(int input);
    int GetKey(InputKeycode input);
    void PrintKey(InputKeycode input, int presstype);
    void PrintKey(InputKeycode input);
    void RemoveKey(KeyIT& input);
    void RemoveHoldKey(KeyIT& input);
  };
}