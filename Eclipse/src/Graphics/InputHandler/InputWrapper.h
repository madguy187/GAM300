#pragma once
#include "AllInputKeyCodes.h"
#include <map>

namespace Eclipse
{
  typedef std::unordered_map< std::pair<InputKeycode, int>, std::pair<bool, InputState> > InputKeyContainer;
  using KeyIT = std::unordered_map< std::pair<InputKeycode, int>, std::pair<bool, InputState> >::iterator;

  class InputWrapper
  {
  private:
    // Trigger and Release Key Container
    InputKeyContainer KeyContainer;

    // Hold and Release Key Container
    InputKeyContainer HoldKeyContainer;

    bool EnablePrint = false;
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

  public:

    // Probably missing this:
    friend std::ostream& operator << (std::ostream& os, const InputKeycode& in);

   InputWrapper();

    // Check if Current Key is triggered
    bool GetKeyTriggered(InputKeycode keycode);

    // Check if Current Key is pressed
    bool GetKeyCurrent(InputKeycode keycode);

    // Get current print flag
    bool GetIsPrint();

    // Set if you want key printings for the keys you pressed
    void SetIsPrint(bool input);

  private:
    void init();

    // Update of Hold Keys
    void Update();

    // Register Trigger Keys into Container
    void RegisterTriggerInput(InputKeycode keycode, int keypressFlag, bool flag, InputState input);

    // Register Hold Keys into Container
    void RegisterHoldInput(InputKeycode keycode, int keypressFlag, bool flag, InputState input);

    // Update Trigger Keys when Released
    bool GetKeyReleased(InputKeycode keycode);

    // Update Hold Keys when Released
    bool GetHoldKeyReleased(InputKeycode keycode);

    // Check if Key is pressed
    bool IsKeyPressed(int input);

    // Check if key released
    bool IsKeyReleased(int input);

    // Get the key's input state
    int GetKey(InputKeycode input);

    // Printing key Code and Message for Trigger and Releasing Key
    void PrintKey(InputKeycode input, int presstype);

    // Printing Key Code and Message for Hold and Releasing Key
    void PrintKey(InputKeycode input);

    // Rremove Triggered Key from Container After Being Released
    void RemoveKey(KeyIT& input);

    // Remove Hold Key from Container After Being Released
    void RemoveHoldKey(KeyIT& input);
  };
}