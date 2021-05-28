#pragma once
#include "AllInputKeyCodes.h"
#include <map>

namespace Eclipse
{
  typedef std::map< std::pair<InputKeycode, int>, std::pair<bool, InputState> > InputKeyContainer;
  using KeyIT = std::map< std::pair<InputKeycode, int>, std::pair<bool, InputState> >::iterator;

  enum class PrintingType
  {
    Null = 0,
    Press = 1,
    Hold = 2,
    Release = 3,
    Maxcount
  };

  class InputWrapper
  {
  private:
    InputKeyContainer KeyContainer;
    InputKeyContainer HoldKeyContainer;

    bool single;
    bool EnablePrint = false;
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

  public:
    void init();
    void Update();
    bool GetKeyTriggered(InputKeycode keycode);
    bool GetKeyCurrent(InputKeycode keycode);
    bool GetIsPrint();
    void SetIsPrint(bool input);

  private:
    bool GetKeyReleased(InputKeycode keycode);
    void RegisterTriggerInput(InputKeycode keycode, int keypressFlag, bool flag, InputState input);
    bool IsKeyPressed(int input);
    bool IsKeyReleased(int input);
    int GetKey(InputKeycode input);
    void PrintKey(InputKeycode input);
    void RemoveKey(KeyIT& input);
  };
}