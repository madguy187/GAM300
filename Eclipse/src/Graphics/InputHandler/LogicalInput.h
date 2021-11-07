#pragma once
#include "AllInputKeyCodes.h"
#include <map>
#include "Graphics/InputHandler/LogicalEnum.hpp"
#include "Graphics/EngineCompiler/InputCompiler/InputCompiler.h"

namespace Eclipse
{
    typedef std::unordered_map< InputKeycode, InputState> InputKeyContainer;
    using KeyIT = std::unordered_map<InputKeycode, InputState>::iterator;

    typedef std::unordered_map< InputMouseKeycode, InputState> MouseContainer;
    using MouseIT = std::unordered_map<InputMouseKeycode, InputState>::iterator;

    class LogicalInput
    {
    private:
        InputKeyContainer KeyContainer;
        MouseContainer MouseContainer_;
        bool EnablePrint = false;
        std::vector<std::string> AllInputs;
        std::vector<std::string> AllMouseInputs;

    public:
        LogicalInput();
        bool GetIsPrint();
        void SetIsPrint(bool input);
        void UpdateKeys();

        ///////////////////////////////////////////////////////////////////////////////////////////
        // For LOGICAL INPUT ( KEYBOARD )
        ///////////////////////////////////////////////////////////////////////////////////////////
        bool GetKeyTriggered(InputKeycode keycode);
        bool GetKeyCurrent(InputKeycode keycode);
        bool GetKeyTriggered(std::string Mappedkeycode);
        bool GetKeyCurrent(std::string Mappedkeycode);
        std::vector<std::string> GetAllKeys();
        std::unordered_map<std::string, InputKeycode> KeyMappings;
        std::vector<std::string> KeyMappings1;

        ///////////////////////////////////////////////////////////////////////////////////////////
        // For LOGICAL INPUT ( MOUSE )
        ///////////////////////////////////////////////////////////////////////////////////////////
        bool GetMouseTriggered(InputMouseKeycode Mappedkeycode);
        bool GetMouseCurrent(InputMouseKeycode Mappedkeycode);
        bool GetMouseTriggered(std::string Mappedkeycode);
        bool GetMouseCurrent(std::string Mappedkeycode);
        std::vector<std::string> GetAllMouseKeys();
        std::unordered_map<std::string, InputMouseKeycode> MouseMappings;

        ///////////////////////////////////////////////////////////////////////////////////////////
        // COMPILER For LOGICAL INPUT 
        ///////////////////////////////////////////////////////////////////////////////////////////
        InputCompiler InputCompiler_;
        std::string GetInputString(const InputKeycode keycode);
        std::string GetInputString(const InputMouseKeycode keycode);
        std::unordered_map<std::string, unsigned int> Dictionary;

        ///////////////////////////////////////////////////////////////////////////////////////////
        // For Nico ( Boolean Checks )
        ///////////////////////////////////////////////////////////////////////////////////////////
        bool CheckKeyboardMapping(std::string NewMap);
        bool CheckMouseMapping(std::string NewMap);
        void TestingLogicalInput(); // For Testing

        ///////////////////////////////////////////////////////////////////////////////////////////
        // GetAxis - mouse
        ///////////////////////////////////////////////////////////////////////////////////////////
        int XMiddle, YMiddle;
        int Mouse_X, Mouse_Y;

        // GetAxis
        float XDelta = 0.0f;
        float YDelta = 0.0f;
        void SetAxis(GLint width, GLint height);
        void CursorUpdate();
        void AxisUpdate();
        float GetAxis(const std::string& Type);
        bool LockCursor(CursorLockMode);
        CursorLockMode State;

        // GetRawAxis
        float XDeltaRaw = 0.0f;
        float YDeltaRaw = 0.0f;
        float GetRawAxis(const std::string& Type);

    private:
        void init();
        void Update();
        void InsertAllKeys();
        InputKeycode CheckMappingExist(std::string);
        InputMouseKeycode CheckMouseMappingExist(std::string NewMap);
        friend std::ostream& operator << (std::ostream& os, const InputKeycode& in);
        void RegisterTriggerInput(InputKeycode keycode, InputState input);
        void RegisterMouseInput(InputMouseKeycode keycode, InputState input);
        void RegisterHoldInput(InputKeycode keycode, InputState input);
        bool GetKeyReleased(InputKeycode keycode);
        bool GetKeyReleased(InputMouseKeycode keycode);
        bool GetHoldKeyReleased(InputMouseKeycode keycode);
        bool GetHoldKeyReleased(InputKeycode keycode);
        bool IsKeyPressed(int input);
        bool IsKeyReleased(int input);
        int GetKey(InputKeycode input);
        int GetKey(InputMouseKeycode input);
        void PrintKey(InputKeycode input, int presstype);
        void PrintKey(InputKeycode input);
        void RemoveKey(KeyIT& input);
        void RemoveHoldKey(KeyIT& input);
        bool CheckPress(int keycode);
    };
}