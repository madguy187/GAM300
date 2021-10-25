#pragma once
#include "AllInputKeyCodes.h"
#include <map>
#include "Graphics/InputHandler/LogicalEnum.hpp"

namespace Eclipse
{
    //typedef std::map< InputKeycode, std::pair<bool, InputState> > InputKeyContainer;
    //using KeyIT = std::map<InputKeycode, std::pair<bool, InputState> >::iterator;
    //typedef std::pair<InputKeycode, std::pair<bool, InputState>> PerKey;
    //using LogicalKeyIT = std::vector<PerKey>::iterator;

    //class InputWrapper
    //{
    //private:
    //    // Trigger and Release Key Container
    //    InputKeyContainer KeyContainer;

    //    // Hold and Release Key Container
    //    InputKeyContainer HoldKeyContainer;

    //    // Logical Input Stuffs

    //    bool EnablePrint = false;
    //    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

    //    unsigned int counter = 0;
    //    ///////////////////////////////////////////////////////////////////////////////////////////
    //    // For LOGICAL INPUT
    //    ///////////////////////////////////////////////////////////////////////////////////////////
    //    // Logical Input Stuffs
    //    std::vector<std::string> AllInputs;

    //public:
    //    // Probably missing this:
    //    friend std::ostream& operator << (std::ostream& os, const InputKeycode& in);
    //    std::string GetInputString(const InputKeycode keycode);

    //    InputWrapper();

    //    // Check if Current Key is triggered
    //    bool GetKeyTriggered(InputKeycode keycode);

    //    // Check if Current Key is pressed
    //    bool GetKeyCurrent(InputKeycode keycode);

    //    // Get current print flag
    //    bool GetIsPrint();

    //    // Set if you want key printings for the keys you pressed
    //    void SetIsPrint(bool input);

    //    ///////////////////////////////////////////////////////////////////////////////////////////
    //    // For LOGICAL INPUT
    //    ///////////////////////////////////////////////////////////////////////////////////////////
    //    // Logical Input Stuffs
    //    std::vector<std::string> KeyMappings1;
    //    std::unordered_map<std::string, InputKeycode> KeyMappings;
    //    bool GetKeyTriggered(std::string);
    //    bool GetKeyCurrent(std::string);
    //    InputKeycode CheckMappingExist(std::string);
    //    void InsertAllKeys();
    //    std::vector<std::string> GetAllKeys();
    //    std::vector<PerKey> LogicalInputContainer;
    //    //bool single = false;
    //    int Press = 0;
    //private:
    //    void init();

    //    // Update of Hold Keys
    //    void Update();

    //    // Register Trigger Keys into Container
    //    void RegisterTriggerInput(InputKeycode keycode, int keypressFlag, bool flag, InputState input);

    //    // Register Hold Keys into Container
    //    void RegisterHoldInput(InputKeycode keycode, int keypressFlag, bool flag, InputState input);

    //    // Update Trigger Keys when Released
    //    bool GetKeyReleased(InputKeycode keycode);

    //    // Update Hold Keys when Released
    //    bool GetHoldKeyReleased(InputKeycode keycode);

    //    // Check if Key is pressed
    //    bool IsKeyPressed(int input);

    //    // Check if key released
    //    bool IsKeyReleased(int input);

    //    // Get the key's input state
    //    int GetKey(InputKeycode input);

    //    // Printing key Code and Message for Trigger and Releasing Key
    //    void PrintKey(InputKeycode input, int presstype);

    //    // Printing Key Code and Message for Hold and Releasing Key
    //    void PrintKey(InputKeycode input);

    //    // Rremove Triggered Key from Container After Being Released
    //    void RemoveKey(KeyIT& input);
    //    void RemoveKey(LogicalKeyIT input);

    //    // Remove Hold Key from Container After Being Released
    //    void RemoveHoldKey(KeyIT& input);
    //};
}