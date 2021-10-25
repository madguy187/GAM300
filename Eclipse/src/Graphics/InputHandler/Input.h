#pragma once
#include "Graphics/InputHandler/Key.h"
#include "Graphics/InputHandler/Log.h"

namespace Eclipse
{
    class GameInputManager
    {
    public:
        GameInputManager() {};
        bool GetKeyPressed(InputKeycode keycode);
        bool GetKeyDown(InputKeycode keycode) const;
        bool GetKeyReleased(InputKeycode keycode) const;
        float GetKeyAxis(InputKeycode keycode) const;

        void Update();

        void Visit(class WinKeyboardInputReciever&, InputKeycode primary, InputState primaryState
            , InputKeycode secondary, InputState secondaryState);
        void Visit(class WinMouseInputReciever&, InputMouseKeycode primary, InputState primaryState
            , InputState crtlState, InputState shiftState, float mouseX, float mouseY);
    private:
        //POD so that we can simply memmove
        struct InputData
        {
            InputState inputState = InputState::Key_NULLSTATE;
            float axisValue = 0.0f;
        };

        std::array<InputData, 256> _keyboardmouseStatus;

        std::array<unsigned char, 256> _pressedKeyRecords;

        unsigned char _currPressedCount = 0;
        unsigned char _lastPressedCount = 0;

        float _mouseScreenX;
        float _mouseScreenY;
    };
}