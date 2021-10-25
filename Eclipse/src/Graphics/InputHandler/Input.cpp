#include "pch.h"
#include "Graphics/InputHandler/Input.h"

namespace Eclipse
{
    bool GameInputManager::GetKeyPressed(InputKeycode keycode)
    {
        unsigned char enumToIndex = static_cast<unsigned char>(keycode);
        return (_keyboardmouseStatus[enumToIndex].inputState == Eclipse::InputState::Key_PRESSED);
    }
    bool Eclipse::GameInputManager::GetKeyDown(Eclipse::InputKeycode keycode) const
    {
        unsigned char enumToIndex = static_cast<unsigned char>(keycode);
        return (_keyboardmouseStatus[enumToIndex].inputState == Eclipse::InputState::Key_HELD);
    }
    bool Eclipse::GameInputManager::GetKeyReleased(Eclipse::InputKeycode keycode) const
    {
        unsigned char enumToIndex = static_cast<unsigned char>(keycode);
        return (_keyboardmouseStatus[enumToIndex].inputState == Eclipse::InputState::Key_RELEASED);
    }

    float Eclipse::GameInputManager::GetKeyAxis(Eclipse::InputKeycode keycode) const
    {
        unsigned char enumToIndex = static_cast<unsigned char>(keycode);
        return _keyboardmouseStatus[enumToIndex].axisValue;
    }

    void Eclipse::GameInputManager::Update()
    {
        //if there was keys pressed last frame (keys located at back of array)
        if (_lastPressedCount > 0)
        {
            size_t arrayStartIndex = 256 - _lastPressedCount;
            // Change their state to held if they were not released
            for (unsigned char i = 0; i < _lastPressedCount; ++i)
            {
                unsigned char keyCodeIndex = _pressedKeyRecords[arrayStartIndex + i];
                if (_keyboardmouseStatus[keyCodeIndex].inputState == InputState::Key_PRESSED)
                {
                    _keyboardmouseStatus[keyCodeIndex].inputState = InputState::Key_HELD;
                }
            }
            //No more pressed keys from last frame
            _lastPressedCount = 0;
        }

        //If there was keys pressed this frame
        if (_currPressedCount > 0)
        {
            //Move them to back of array
            size_t arrayStartIndex = 256 - _currPressedCount;

            //we use std::memcpy instead of std::memmove since keys should not overlap (you cant press the same key over two frames)
            //memmove is slightly slower since it need to handle overlap... and it uses an extra buffer when moving
            std::memcpy(&(_pressedKeyRecords[arrayStartIndex]), &(_pressedKeyRecords[0]), sizeof(unsigned char) * _currPressedCount);

            //Set the number of pressed key that will retain until next frame
            _lastPressedCount = _currPressedCount;
        }
        //Reset the current pressed count for next frame
        _currPressedCount = 0;
    }

    void Eclipse::GameInputManager::Visit(WinKeyboardInputReciever&, InputKeycode primary, InputState primaryState
        , InputKeycode secondary, InputState secondaryState)
    {
        //Register or Unregister the keys
        if (primary != InputKeycode::KeyCode_NULL)
        {
            std::cout << primary << std::endl;

            unsigned char enumToIndex = static_cast<unsigned char>(primary);
            if (primaryState == InputState::Key_PRESSED)
            {
                std::cout << "Pressed" << std::endl;
                _keyboardmouseStatus[enumToIndex].axisValue = 1.0f;              //For window just press all the way down

                // Record that the current key was pressed in current frame
                _pressedKeyRecords[_currPressedCount++] = enumToIndex;
            }
            else if (primaryState == InputState::Key_RELEASED)
            {
                std::cout << "Released" << std::endl;
                _keyboardmouseStatus[enumToIndex].axisValue = 0.0f;              //For window just release all the way up
            }
            else if (primaryState == InputState::Key_HELD)
            {
                std::cout << "Held" << std::endl;
                // Remain the same value
            }
            _keyboardmouseStatus[enumToIndex].inputState = primaryState;
        }

        if (secondary != InputKeycode::KeyCode_NULL)
        {
            std::cout << secondary << std::endl;

            unsigned char enumToIndex = static_cast<unsigned char>(secondary);
            if (secondaryState == InputState::Key_PRESSED)
            {
                std::cout << "Pressed" << std::endl;
                _keyboardmouseStatus[enumToIndex].axisValue = 1.0f;              //For window just press all the way down

                // Record that the current key was pressed in current frame
                _pressedKeyRecords[_currPressedCount++] = enumToIndex;
            }
            else if (secondaryState == InputState::Key_RELEASED)
            {
                std::cout << "Released" << std::endl;
                _keyboardmouseStatus[enumToIndex].axisValue = 0.0f;              //For window just release all the way up
            }
            else if (secondaryState == InputState::Key_HELD)
            {
                std::cout << "Held" << std::endl;
                // Remain the same value
            }
            _keyboardmouseStatus[enumToIndex].inputState = secondaryState;
        }
    }

    void Eclipse::GameInputManager::Visit(class WinMouseInputReciever&, InputMouseKeycode primary, InputState primaryState
        , InputState crtlState, InputState shiftState, float mouseX, float mouseY)
    {
        //std::cout << "Mouse Screen Position: x = " << mouseX << " y = " << mouseY << std::endl;
        _mouseScreenX = mouseX;
        _mouseScreenY = mouseY;

        //Register or Unregister the keys
        if (primary != InputMouseKeycode::KeyCode_MOUSENULL)
        {
            unsigned char enumToIndex = static_cast<unsigned char>(primary);
            //std::cout << primary << std::endl;
            if (primaryState == InputState::Key_PRESSED)
            {
                //std::cout << "Pressed" << std::endl;
                _keyboardmouseStatus[enumToIndex].axisValue = 1.0f;              //For window just press all the way down

                if (_keyboardmouseStatus[enumToIndex].inputState != InputState::Key_HELD)
                {
                    // Record that the current key was pressed in current frame
                    _pressedKeyRecords[_currPressedCount++] = enumToIndex;
                    _keyboardmouseStatus[enumToIndex].inputState = primaryState;
                }
            }
            else if (primaryState == InputState::Key_RELEASED)
            {
                //std::cout << "Released" << std::endl;
                _keyboardmouseStatus[enumToIndex].axisValue = 0.0f;              //For window just release all the way up
                _keyboardmouseStatus[enumToIndex].inputState = primaryState;
            }
        }

        //_keyboardmouseStatus[static_cast<unsigned char>(InputKeycode::KeyCode_CTRL)].inputState = crtlState;
        //_keyboardmouseStatus[static_cast<unsigned char>(InputKeycode::KeyCode_CTRL)].axisValue = (crtlState == InputState::Key_RELEASED ? 0.0f : 1.0f);
        //_keyboardmouseStatus[static_cast<unsigned char>(InputKeycode::KeyCode_SHIFT)].inputState = shiftState;
        //_keyboardmouseStatus[static_cast<unsigned char>(InputKeycode::KeyCode_SHIFT)].axisValue = (shiftState == InputState::Key_RELEASED ? 0.0f : 1.0f);

    }
}