#include "pch.h"
#include "LogicalInput.h"
#include "Graphics/OpenGL/OpenGL_Context.h"
#include "Editor/Windows/GameView/GameView.h"

///////////////////////////////////////////////////////////////////////////////////////////
// For LOGICAL INPUT ( KEYBOARD )
///////////////////////////////////////////////////////////////////////////////////////////
namespace Eclipse
{
    bool LogicalInput::GetKeyTriggered(InputKeycode keycode)
    {
        int Press = GetKey(keycode);

        if (IsKeyReleased(Press))
        {
            GetKeyReleased(keycode);
            return false;
        }
        // Check if Space is pressed
        else if (IsKeyPressed(Press))
        {
            RegisterTriggerInput(keycode, InputState::Key_TRIGGERED);

            if (KeyContainer.count(keycode) != 0 && KeyContainer[keycode].inputState == InputState::Key_TRIGGERED)
            {
                PrintKey(keycode, Press);
                return true;
            }
        }

        return false;
    }

    bool LogicalInput::GetKeyCurrent(InputKeycode keycode)
    {
        int Hold = GetKey(keycode);

        if (IsKeyReleased(Hold))
        {
            GetHoldKeyReleased(keycode);
            return false;
        }
        else if (IsKeyPressed(Hold))
        {
            RegisterHoldInput(keycode, InputState::Key_TRIGGERED);

            if (KeyContainer.count(keycode) != 0 &&
                (KeyContainer[keycode].inputState == InputState::Key_HOLD || KeyContainer[keycode].inputState == InputState::Key_HOLD))
            {
                //PrintKey(keycode, Hold);
                return true;
            }
        }

        return false;
    }

    bool LogicalInput::GetKeyTriggered(std::string Mappedkeycode)
    {
        // Lets see what Key is this
        InputKeycode PassedIn = CheckMappingExist(Mappedkeycode);

        // Check whether the string passed in is mapped with key
        int Press = GetKey(PassedIn);

        //Check if key is released
        if (IsKeyReleased(Press))
        {
            GetKeyReleased(PassedIn);
            return false;
        }
        // Check if Key is pressed
        else if (IsKeyPressed(Press))
        {
            if (Press != 1)
                return false;

            RegisterTriggerInput(PassedIn, InputState::Key_TRIGGERED);

            if (KeyContainer.count(PassedIn) && KeyContainer[PassedIn].inputState == InputState::Key_TRIGGERED)
            {
                PrintKey(PassedIn, Press);
                return true;
            }
        }

        return false;
    }

    bool LogicalInput::GetKeyCurrent(std::string Mappedkeycode)
    {
        // Lets see what Key is this
        InputKeycode PassedIn = CheckMappingExist(Mappedkeycode);

        int Hold = GetKey(PassedIn);

        //Check if key is released
        if (IsKeyReleased(Hold))
        {
            GetHoldKeyReleased(PassedIn);
            return false;
        }
        //Check if key is pressed
        else if (IsKeyPressed(Hold))
        {
            if (Hold != 1)
                return false;

            RegisterHoldInput(PassedIn, InputState::Key_HOLD);

            if (KeyContainer.count(PassedIn) != 0 &&
                (KeyContainer[PassedIn].inputState == InputState::Key_TRIGGERED || KeyContainer[PassedIn].inputState == InputState::Key_HOLD))
            {
                //PrintKey(keycode, Hold);
                return true;
            }
        }

        return false;
    }

    InputKeycode LogicalInput::CheckMappingExist(std::string NewMap)
    {
        for (const auto& i : KeyMappings)
        {
            if (!strcmp(i.first.c_str(), NewMap.c_str()))
            {
                if (i.second != InputKeycode::Key_Null)
                {
                    return i.second;
                }
            }
        }

        return InputKeycode::Key_Null;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
// For LOGICAL INPUT ( MOUSE )
///////////////////////////////////////////////////////////////////////////////////////////
namespace Eclipse
{
    bool LogicalInput::GetMouseTriggered(InputMouseKeycode Mappedkeycode)
    {
        int Press = GetKey(Mappedkeycode);

        if (IsKeyReleased(Press))
        {
            GetKeyReleased(Mappedkeycode);
            return false;
        }
        else if (IsKeyPressed(Press))
        {
            RegisterMouseInput(Mappedkeycode, InputState::Key_TRIGGERED);

            if (MouseContainer_.count(Mappedkeycode) != 0 &&
                MouseContainer_[Mappedkeycode].inputState == InputState::Key_TRIGGERED)
            {
                return true;
            }
        }

        return false;
    }

    bool LogicalInput::GetMouseCurrent(InputMouseKeycode Mappedkeycode)
    {
        int Hold = GetKey(Mappedkeycode);

        if (IsKeyReleased(Hold))
        {
            GetHoldKeyReleased(Mappedkeycode);
            return false;
        }
        else if (IsKeyPressed(Hold))
        {
            RegisterMouseInput(Mappedkeycode, InputState::Key_HOLD);

            if (MouseContainer_.count(Mappedkeycode) != 0 &&
                (MouseContainer_[Mappedkeycode].inputState == InputState::Key_TRIGGERED || MouseContainer_[Mappedkeycode].inputState == InputState::Key_HOLD))
            {
                return true;
            }
        }

        return false;
    }

    bool LogicalInput::GetMouseTriggered(std::string Mappedkeycode)
    {
        // Lets see what Key is this
        InputMouseKeycode PassedIn = CheckMouseMappingExist(Mappedkeycode);

        int Press = GetKey(PassedIn);

        if (IsKeyReleased(Press))
        {
            GetKeyReleased(PassedIn);
            return false;
        }
        else if (IsKeyPressed(Press))
        {
            if (Press != 1)
                return false;

            RegisterMouseInput(PassedIn, InputState::Key_TRIGGERED);

            if (MouseContainer_.count(PassedIn) != 0 &&
                MouseContainer_[PassedIn].inputState == InputState::Key_TRIGGERED)
            {
                return true;
            }
        }

        return false;
    }

    bool LogicalInput::GetMouseCurrent(std::string Mappedkeycode)
    {
        InputMouseKeycode PassedIn = CheckMouseMappingExist(Mappedkeycode);

        int Hold = GetKey(PassedIn);

        if (IsKeyReleased(Hold))
        {
            GetHoldKeyReleased(PassedIn);
            return false;
        }
        else if (IsKeyPressed(Hold))
        {
            if (Hold != 1)
                return false;

            RegisterMouseInput(PassedIn, InputState::Key_HOLD);

            if (MouseContainer_.count(PassedIn) != 0 &&
                (MouseContainer_[PassedIn].inputState == InputState::Key_TRIGGERED || MouseContainer_[PassedIn].inputState == InputState::Key_HOLD))
            {
                return true;
            }
        }
        return false;
    }

    InputMouseKeycode LogicalInput::CheckMouseMappingExist(std::string NewMap)
    {
        for (const auto& i : MouseMappings)
        {
            if (!strcmp(i.first.c_str(), NewMap.c_str()))
            {
                if (i.second != InputMouseKeycode::KeyCode_NONE)
                {
                    return i.second;
                }
            }
        }

        return InputMouseKeycode::KeyCode_NONE;
    }
}

namespace Eclipse
{
    LogicalInput::LogicalInput()
    {
        EnablePrint = false;

        init();
        InsertAllKeys();

        //KeyMappings.emplace("Horizontal", InputKeycode::Key_LEFT);
        //MouseMappings.emplace("Hello", InputMouseKeycode::KeyCode_MOUSELEFT);

        // Set Cursor to middle
        float ratioX = 1850.0f / 1270.0f;
        float ratioY = 970.0f / 593.0f;
        float MiddleX = (1850.0f / 2) / ratioX;
        float MiddleY = (970.0f / 2) / ratioY;
        //glfwSetCursorPos(OpenGL_Context::ptr_window, MiddleX, MiddleY);

        // Prepare Axis
        XMiddle = MiddleX; // (OpenGL_Context::GetWidth() / 2) / ratioX;
        YMiddle = MiddleY; // (OpenGL_Context::GetHeight() / 2) / ratioY;
    }

    void LogicalInput::InsertAllKeys()
    {
        for (unsigned int i = 0; i <= static_cast<unsigned int>(Eclipse::InputKeycode::Key_APPS); i++)
        {
            std::string Key = GetInputString(static_cast<Eclipse::InputKeycode>(i)).data();

            if (Key.empty() == false)
            {
                AllInputs.push_back(Key);
                Dictionary.emplace(Key, i);
            }
        }

        for (unsigned char i = 0; i <= static_cast<unsigned char>(Eclipse::InputMouseKeycode::KeyCode_MOUSERIGHT); i++)
        {
            std::string Key = GetInputString(static_cast<Eclipse::InputKeycode>(i)).data();

            if (Key.empty() == false)
            {
                AllMouseInputs.push_back(Key);
            }
        }
    }

    std::vector<std::string> LogicalInput::GetAllKeys()
    {
        return AllInputs;
    }

    std::vector<std::string> LogicalInput::GetAllMouseKeys()
    {
        return AllMouseInputs;
    }

    bool LogicalInput::CheckKeyboardMapping(std::string NewMap)
    {
        for (const auto& i : KeyMappings)
        {
            if (!strcmp(i.first.c_str(), NewMap.c_str()))
            {
                if (i.second != InputKeycode::Key_Null)
                {
                    return true;
                }
            }
        }

        return false;
    }

    bool LogicalInput::CheckMouseMapping(std::string NewMap)
    {
        for (const auto& i : MouseMappings)
        {
            if (!strcmp(i.first.c_str(), NewMap.c_str()))
            {
                if (i.second != InputMouseKeycode::KeyCode_NONE)
                {
                    return true;
                }
            }
        }

        return false;
    }

    void LogicalInput::TestingLogicalInput()
    {
        if (engine->InputManager->GetKeyTriggered("Left"))
        {
            std::cout << "Left" << std::endl;
        }

        if (engine->InputManager->GetKeyTriggered("Right"))
        {
            std::cout << "Right" << std::endl;
        }

        if (engine->InputManager->GetKeyTriggered("Up"))
        {
            std::cout << "Up" << std::endl;
        }

        if (engine->InputManager->GetKeyTriggered("Down"))
        {
            std::cout << "Down" << std::endl;
        }
    }

    float LogicalInput::GetRawAxis(const std::string& Type)
    {
        if (Type == "Mouse X")
        {
            return XDeltaRaw;
        }
        else if (Type == "Mouse Y")
        {
            return YDeltaRaw;
        }

        if (Type == "Horizontal")
        {
            if (GetKeyCurrent(InputKeycode::Key_A) || GetKeyCurrent(InputKeycode::Key_LEFT))
            {
                return XDeltaKeyRaw = -1;
            }
            else if (GetKeyCurrent(InputKeycode::Key_D) || GetKeyCurrent(InputKeycode::Key_RIGHT))
            {
                return XDeltaKeyRaw = 1;
            }
            else
            {
                return XDeltaKeyRaw = 0;
            }
        }

        if (Type == "Vertical")
        {
            if (GetKeyCurrent(InputKeycode::Key_W) || GetKeyCurrent(InputKeycode::Key_UP))
            {
                return YDeltaKeyRaw = -1;
            }
            else if (GetKeyCurrent(InputKeycode::Key_S) || GetKeyCurrent(InputKeycode::Key_DOWN))
            {
                return YDeltaKeyRaw = 1;
            }
            else
            {
                return YDeltaKeyRaw = 0;
            }
        }


        return 0.0f;
    }

    void LogicalInput::init()
    {
        // Clear Container just in case
        KeyContainer.clear();
    }

    bool LogicalInput::GetIsPrint()
    {
        return EnablePrint;
    }

    void LogicalInput::SetIsPrint(bool input)
    {
        EnablePrint = input;
    }

    void LogicalInput::RegisterTriggerInput(InputKeycode keycode, InputState input)
    {
        InputData NewInputData(input, true);
        KeyContainer.insert({ keycode, NewInputData });
    }

    void LogicalInput::RegisterMouseInput(InputMouseKeycode keycode, InputState input)
    {
        InputData NewInputData(input, true);
        MouseContainer_.insert({ keycode, NewInputData });
    }

    void LogicalInput::RegisterHoldInput(InputKeycode keycode, InputState input)
    {
        InputData NewInputData(input, false);
        KeyContainer.insert({ keycode, NewInputData });

        //KeyContainer.insert({ keycode, input });
    }

    bool LogicalInput::IsKeyPressed(int input)
    {
        return (input == GLFW_PRESS);
    }

    bool LogicalInput::IsKeyReleased(int input)
    {
        return (input == GLFW_RELEASE);
    }

    int LogicalInput::GetKey(InputKeycode input)
    {
        return (glfwGetKey(OpenGL_Context::GetWindow(), static_cast<int>(input)));
    }

    int LogicalInput::GetKey(InputMouseKeycode input)
    {
        return (glfwGetMouseButton(OpenGL_Context::GetWindow(), static_cast<int>(input)));
    }

    void LogicalInput::PrintKey(InputKeycode input, int presstype)
    {
        if (EnablePrint)
        {

            switch (presstype)
            {

            case GLFW_PRESS:
            {
                std::cout << "Pressed " << (input) << " --> " << "KeyCode " << static_cast<int>(input) << std::endl;
            }
            break;

            case GLFW_RELEASE:
            {
                std::cout << "Released " << (input) << " --> " << "KeyCode " << static_cast<int>(input) << std::endl;
            }
            break;

            default:
                break;
            }
        }
    }

    void LogicalInput::PrintKey(InputKeycode input)
    {
        if (EnablePrint)
        {
            std::cout << "Holding " << (input) << " --> " << "KeyCode " << static_cast<int>(input) << std::endl;
            //std::cout << "Container Size : " << HoldKeyContainer.size() << std::endl;
        }
    }

    void LogicalInput::RemoveKey(KeyIT& input)
    {
        KeyContainer.erase(input);
    }

    void LogicalInput::UpdateKeys()
    {
        for (auto& pair2 : KeyContainer)
        {
            //// Horizontal
            //if (pair2.first == InputKeycode::Key_LEFT || pair2.first == InputKeycode::Key_A)
            //{
            //    if (pair2.second.inputState == InputState::Key_HOLD && XDeltaKey >= -1.0f)
            //    {
            //        XDeltaKey -= engine->Game_Clock.get_DeltaTime();
            //    }
            //    XDeltaKeyRaw = -1;
            //}
            //
            //if (pair2.first == InputKeycode::Key_RIGHT || pair2.first == InputKeycode::Key_D)
            //{
            //    if (pair2.second.inputState == InputState::Key_HOLD && XDeltaKey <= 1.0f)
            //    {
            //        XDeltaKey += engine->Game_Clock.get_DeltaTime();
            //    }
            //
            //    XDeltaKeyRaw = 1.0;
            //}
            //
            //// Vertical
            //// Right and down is positive
            //// left and up is negative
            //if (pair2.first == InputKeycode::Key_UP || pair2.first == InputKeycode::Key_W)
            //{
            //    if (pair2.second.inputState == InputState::Key_HOLD && YDeltaKey > -1.0f)
            //    {
            //        //std::cout << "Test" << std::endl;
            //        YDeltaKey -= engine->Game_Clock.get_DeltaTime();
            //    }
            //
            //    YDeltaKeyRaw = -1;
            //}
            //
            //if (pair2.first == InputKeycode::Key_DOWN || pair2.first == InputKeycode::Key_S)
            //{
            //    if (pair2.second.inputState == InputState::Key_HOLD && YDeltaKey < 1.0f)
            //    {
            //        YDeltaKey += engine->Game_Clock.get_DeltaTime();
            //    }
            //    YDeltaKeyRaw = 1;
            //}

            if (pair2.second.inputState == InputState::Key_TRIGGERED)
            {
                pair2.second.inputState = InputState::Key_HOLD;
            }
        }

        // Mouse
        for (auto& pair2 : MouseContainer_)
        {
            if (pair2.second.inputState == InputState::Key_TRIGGERED)
            {
                pair2.second.inputState = InputState::Key_HOLD;
            }
        }

        //for (auto& pair3 : ReleaseContainer)
        //{
        //    if (pair3.first == InputKeycode::Key_LEFT || pair3.first == InputKeycode::Key_A)
        //    {
        //        if (RawAxisFlag == true)
        //        {
        //            XDeltaKeyRaw = 0;
        //            DeleteContainer.push_back(pair3.first);
        //            continue;
        //        }

        //        if (XDeltaKey < 0.0)
        //        {
        //            XDeltaKey += engine->Game_Clock.get_DeltaTime();
        //        }
        //        else
        //        {
        //            XDeltaKey = 0.0;
        //            DeleteContainer.push_back(pair3.first);
        //        }
        //    }

        //    if (pair3.first == InputKeycode::Key_RIGHT || pair3.first == InputKeycode::Key_D)
        //    {
        //        if (RawAxisFlag == true)
        //        {
        //            XDeltaKeyRaw = 0;
        //            DeleteContainer.push_back(pair3.first);
        //            continue;
        //        }

        //        if (XDeltaKey > 0.0)
        //        {
        //            XDeltaKey -= engine->Game_Clock.get_DeltaTime();
        //        }
        //        else
        //        {
        //            XDeltaKey = 0.0;
        //            DeleteContainer.push_back(pair3.first);
        //        }
        //    }

        //    if (pair3.first == InputKeycode::Key_UP || pair3.first == InputKeycode::Key_W)
        //    {
        //        if (pair3.second.Trigger == false)
        //        {
        //            YDeltaKeyRaw = 0;
        //            DeleteContainer.push_back(pair3.first);
        //            continue;
        //        }

        //        if (YDeltaKey < 0.0)
        //        {
        //            YDeltaKey += engine->Game_Clock.get_DeltaTime();
        //        }
        //        else
        //        {
        //            YDeltaKey = 0.0;
        //            YDeltaKeyRaw = 0;
        //            DeleteContainer.push_back(pair3.first);
        //        }
        //    }

        //    if (pair3.first == InputKeycode::Key_DOWN || pair3.first == InputKeycode::Key_S)
        //    {
        //        if (RawAxisFlag == true)
        //        {
        //            YDeltaKeyRaw = 0;
        //            DeleteContainer.push_back(pair3.first);
        //            continue;
        //        }

        //        if (YDeltaKey > 0.0)
        //        {
        //            YDeltaKey -= engine->Game_Clock.get_DeltaTime();
        //        }
        //        else
        //        {
        //            YDeltaKey = 0.0;
        //            YDeltaKeyRaw = 0;
        //            DeleteContainer.push_back(pair3.first);
        //        }
        //    }
        //}

        //if (DeleteContainer.size() != 0)
        //{
        //    for (auto& i : DeleteContainer)
        //    {
        //        ReleaseContainer.erase(i);
        //    }

        //    RawAxisFlag = false;
        //    DeleteContainer.clear();
        //}
    }

    bool LogicalInput::GetKeyReleased(InputKeycode keycode)
    {
        int Release = GetKey(keycode);

        if (IsKeyReleased(Release))
        {
            if (KeyContainer.count(keycode) != 0)
            {
                ReleaseContainer.emplace(keycode, KeyContainer[keycode]);

                PrintKey(keycode, Release);
                KeyContainer.erase(keycode);
                return true;
            }
            else
            {
                return false;
            }
        }
        return false;
    }

    bool LogicalInput::GetKeyReleased(InputMouseKeycode keycode)
    {
        int Release = GetKey(keycode);

        if (IsKeyReleased(Release))
        {
            if (MouseContainer_.count(keycode) != 0)
            {
                MouseContainer_.erase(keycode);
                return true;
            }
            else
            {
                return false;
            }
        }
        return false;
    }

    bool LogicalInput::GetHoldKeyReleased(InputMouseKeycode keycode)
    {
        int HoldKeyRelease = GetKey(keycode);

        if (IsKeyReleased(HoldKeyRelease))
        {
            if (MouseContainer_.count(keycode) != 0)
            {
                //PrintKey(keycode, HoldKeyRelease);
                MouseContainer_.erase(keycode);
                return true;
            }
            else
            {
                return false;
            }
        }
        return false;

    }

    bool LogicalInput::GetHoldKeyReleased(InputKeycode keycode)
    {
        int HoldKeyRelease = GetKey(keycode);

        if (IsKeyReleased(HoldKeyRelease))
        {
            if (KeyContainer.count(keycode) != 0)
            {
                ReleaseContainer.emplace(keycode, KeyContainer[keycode]);
                //PrintKey(keycode, HoldKeyRelease);
                KeyContainer.erase(keycode);
                return true;
            }
            else
            {
                return false;
            }
        }
        return false;
    }
}

namespace Eclipse
{

    bool LogicalInput::CheckPress(int Press_)
    {
        if (Press_ != 1)
            return false;

        return true;
    }

    std::ostream& operator<<(std::ostream& stream, const InputKeycode& keycode)
    {
        switch (keycode)
        {
        case InputKeycode::Key_Null:
            stream << "Key_NULL";
            break;

            /*************************************************************************
              Keyboard Alphabets Layout
            *************************************************************************/
        case InputKeycode::Key_A:
            stream << "Key_A";
            break;
        case InputKeycode::Key_B:
            stream << "Key_B";
            break;
        case InputKeycode::Key_C:
            stream << "Key_C";
            break;
        case InputKeycode::Key_D:
            stream << "Key_D";
            break;
        case InputKeycode::Key_E:
            stream << "Key_E";
            break;
        case InputKeycode::Key_F:
            stream << "Key_F";
            break;
        case InputKeycode::Key_G:
            stream << "Key_G";
            break;
        case InputKeycode::Key_H:
            stream << "Key_H";
            break;
        case InputKeycode::Key_I:
            stream << "Key_I";
            break;
        case InputKeycode::Key_J:
            stream << "Key_J";
            break;
        case InputKeycode::Key_K:
            stream << "Key_K";
            break;
        case InputKeycode::Key_L:
            stream << "Key_L";
            break;
        case InputKeycode::Key_M:
            stream << "Key_M";
            break;
        case InputKeycode::Key_N:
            stream << "Key_N";
            break;
        case InputKeycode::Key_O:
            stream << "Key_O";
            break;
        case InputKeycode::Key_P:
            stream << "Key_P";
            break;
        case InputKeycode::Key_Q:
            stream << "Key_Q";
            break;
        case InputKeycode::Key_R:
            stream << "Key_R";
            break;
        case InputKeycode::Key_S:
            stream << "Key_S";
            break;
        case InputKeycode::Key_T:
            stream << "Key_T";
            break;
        case InputKeycode::Key_U:
            stream << "Key_U";
            break;
        case InputKeycode::Key_V:
            stream << "Key_V";
            break;
        case InputKeycode::Key_W:
            stream << "Key_W";
            break;
        case InputKeycode::Key_X:
            stream << "Key_X";
            break;
        case InputKeycode::Key_Y:
            stream << "Key_Y";
            break;
        case InputKeycode::Key_Z:
            stream << "Key_Z";
            break;

            /*************************************************************************
              Keyboard Numbers Layout
            *************************************************************************/
        case InputKeycode::Key_0:
            stream << "Key_0";
            break;
        case InputKeycode::Key_1:
            stream << "Key_1";
            break;
        case InputKeycode::Key_2:
            stream << "Key_2";
            break;
        case InputKeycode::Key_3:
            stream << "Key_3";
            break;
        case InputKeycode::Key_4:
            stream << "Key_4";
            break;
        case InputKeycode::Key_5:
            stream << "Key_5";
            break;
        case InputKeycode::Key_6:
            stream << "Key_6";
            break;
        case InputKeycode::Key_7:
            stream << "Key_7";
            break;
        case InputKeycode::Key_8:
            stream << "Key_8";
            break;
        case InputKeycode::Key_9:
            stream << "Key_9";
            break;

            /*************************************************************************
              KeyPad Keys Layout ( The one on the right of your keyboard )
            *************************************************************************/
        case InputKeycode::KeyPad_ENTER:
            stream << "KeyPad Enter";
            break;
        case InputKeycode::KeyPad_EQUAL:
            stream << "KeyPad Equal";
            break;
        case InputKeycode::KeyPad_MULTIPLY:
            stream << "KeyPad Multiply";
            break;
        case InputKeycode::KeyPad_ADD:
            stream << "KeyPad Add";
            break;
        case InputKeycode::KeyPad_SEPERATOR:
            stream << "KeyPad Seperator";
            break;
        case InputKeycode::KeyPad_SUBTRACT:
            stream << "KeyPad Subtract";
            break;
        case InputKeycode::KeyPad_DECIMAL:
            stream << "KeyPad Decimal";
            break;
        case InputKeycode::KeyPad_DIVIDE:
            stream << "KeyPad Divide";
            break;

            /*************************************************************************
              Keyboard F Keys Layout
            *************************************************************************/
        case InputKeycode::Key_F1:
            stream << "Key_F1";
            break;
        case InputKeycode::Key_F2:
            stream << "Key_F2";
            break;
        case InputKeycode::Key_F3:
            stream << "Key_F3";
            break;
        case InputKeycode::Key_F4:
            stream << "Key_F4";
            break;
        case InputKeycode::Key_F5:
            stream << "Key_F5";
            break;
        case InputKeycode::Key_F6:
            stream << "Key_F6";
            break;
        case InputKeycode::Key_F7:
            stream << "Key_F7";
            break;
        case InputKeycode::Key_F8:
            stream << "Key_F8";
            break;
        case InputKeycode::Key_F9:
            stream << "Key_F9";
            break;
        case InputKeycode::Key_F10:
            stream << "Key_F10";
            break;
        case InputKeycode::Key_F11:
            stream << "Key_F11";
            break;
        case InputKeycode::Key_F12:
            stream << "Key_F12";
            break;

            /*************************************************************************
              Keyboard Arrow Keys Layout
            *************************************************************************/
        case InputKeycode::Key_LEFT:
            stream << "Key_LEFT";
            break;
        case InputKeycode::Key_UP:
            stream << "Key_UP";
            break;
        case InputKeycode::Key_RIGHT:
            stream << "Key_RIGHT";
            break;
        case InputKeycode::Key_DOWN:
            stream << "Key_DOWN";
            break;

            /*************************************************************************
              Keyboard OTHER Keys Layout
            *************************************************************************/
        case InputKeycode::Key_BACKSPACE:
            stream << "Key_BACKSPACE";
            break;
        case InputKeycode::Key_TAB:
            stream << "Key_TAB";
            break;
        case InputKeycode::Key_PAUSE:
            stream << "Key_PAUSE";
            break;
        case InputKeycode::Key_CAPSLOCK:
            stream << "Key_CapsLock";
            break;
        case InputKeycode::Key_ESC:
            stream << "Key_ESC";
            break;
        case InputKeycode::Key_SPACE:
            stream << "Key_SPACE";
            break;
        case InputKeycode::Key_ENTER:
            stream << "Key_Enter";
            break;
        case InputKeycode::Key_INSERT:
            stream << "Key_INSERT";
            break;
        case InputKeycode::Key_DELETE:
            stream << "Key_DELETE";
            break;
        case InputKeycode::Key_NUMLOCK:
            stream << "Key_NUMLOCK";
            break;
        case InputKeycode::Key_SHIFTLEFT:
            stream << "Key_SHIFTLEFT";
            break;
        case InputKeycode::Key_SHIFTRIGHT:
            stream << "Key_SHIFTRIGHT";
            break;
        case InputKeycode::Key_CTRLLEFT:
            stream << "Key_CTRLLEFT";
            break;
        case InputKeycode::Key_CTRLRIGHT:
            stream << "Key_CTRLRIGHT";
            break;
        case InputKeycode::Key_ALTLEFT:
            stream << "Key_ALTLEFT";
            break;
        case InputKeycode::Key_ALTRIGHT:
            stream << "Key_ALTRIGHT";
            break;
        case InputKeycode::Key_SEMICOLON:
            stream << "Key_SEMICOLON";
            break;
        case InputKeycode::Key_COMMA:
            stream << "Key_COMMA";
            break;
        case InputKeycode::Key_MINUS:
            stream << "Key_MINUS";
            break;
        case InputKeycode::Key_EQUAL:
            stream << "Key_EQUAL";
            break;
        case InputKeycode::Key_FORWARDSLASH:
            stream << "Key_FORWARDSLASH";
            break;
        case InputKeycode::Key_SQBRACKETLEFT:
            stream << "Key_SQBRACKETLEFT";
            break;
        case InputKeycode::Key_BACKSLASH:
            stream << "Key_BACKSLASH ";
            break;
        case InputKeycode::Key_SQBRACKETRIGHT:
            stream << "Key_SQBRACKETRIGHT";
            break;

            // cant work on my stupid ass laptop
        case InputKeycode::Key_CLEAR:
            stream << "Key_CLEAR";
            break;
        case InputKeycode::Key_RETURN:
            stream << "Key_RETURN";
            break;
        case InputKeycode::Key_SHIFT:
            stream << "Key_SHIFT";
            break;
        case InputKeycode::Key_CTRL:
            stream << "Key_CTRL";
            break;
        case InputKeycode::Key_ALT:
            stream << "Key_ALT";
            break;
        case InputKeycode::Key_PGUP:
            stream << "Key_PGUP";
            break;
        case InputKeycode::Key_PGDN:
            stream << "Key_PGDN";
            break;
        case InputKeycode::Key_END:
            stream << "Key_END ";
            break;
        case InputKeycode::Key_HOME:
            stream << "Key_HOME";
            break;
        case InputKeycode::Key_SELECT:
            stream << "Key_SELECT";
            break;
        case InputKeycode::Key_PRINT:
            stream << "Key_PRINT";
            break;
        case InputKeycode::Key_EXECUTE:
            stream << "Key_EXECUTE";
            break;
        case InputKeycode::Key_PRTSCREEN:
            stream << "Key_PRTSC";
            break;
        case InputKeycode::Key_HELP:
            stream << "Key_HELP";
            break;
        case InputKeycode::Key_APPS:
            stream << "Key_APPS";
            break;
        case InputKeycode::Key_SLEEP:
            stream << "Key_SLEEP";
            break;
        case InputKeycode::Key_MULTIPLY:
            stream << "Key_MULTIPLY";
            break;
        case InputKeycode::Key_ADD:
            stream << "Key_ADD";
            break;
        case InputKeycode::Key_SEPERATOR:
            stream << "Key_SEPERATOR";
            break;
        case InputKeycode::Key_SUBTRACT:
            stream << "Key_SUBTRACT";
            break;
        case InputKeycode::Key_DECIMAL:
            stream << "Key_DECIMAL";
            break;
        case InputKeycode::Key_DIVIDE:
            stream << "Key_DIVIDE";
            break;
        case InputKeycode::Key_SCROLLLOCK:
            stream << "Key_SCROLLLOCK";
            break;
        case InputKeycode::Key_PLUS:
            stream << "Key_PLUS";
            break;
        case InputKeycode::Key_QUOTE:
            stream << "Key_QUOTE";
            break;
        case InputKeycode::Key_MISCELLANEOUS:
            stream << "Key_MISCELLANEOUS";
            break;
        default:
            break;
        }
        return stream;
    }

    std::string LogicalInput::GetInputString(const InputMouseKeycode keycode)
    {
        switch (keycode)
        {
        case InputMouseKeycode::KeyCode_MOUSELEFT:
            return "KeyCode_MOUSELEFT";
            break;
        case InputMouseKeycode::KeyCode_MOUSERIGHT:
            return "KeyCode_MOUSERIGHT";
            break;

        default:
            break;
        }

        return "";
    }

    std::string LogicalInput::GetInputString(const InputKeycode keycode)
    {
        switch (keycode)
        {
        case InputKeycode::Key_Null:
            return "Key_NULL";
            break;
            /*************************************************************************
              Keyboard Alphabets Layout
            *************************************************************************/
        case InputKeycode::Key_A:
            return "Key_A";
            break;
        case InputKeycode::Key_B:
            return "Key_B";
            break;
        case InputKeycode::Key_C:
            return "Key_C";
            break;
        case InputKeycode::Key_D:
            return "Key_D";
            break;
        case InputKeycode::Key_E:
            return "Key_E";
            break;
        case InputKeycode::Key_F:
            return "Key_F";
            break;
        case InputKeycode::Key_G:
            return "Key_G";
            break;
        case InputKeycode::Key_H:
            return "Key_H";
            break;
        case InputKeycode::Key_I:
            return "Key_I";
            break;
        case InputKeycode::Key_J:
            return "Key_J";
            break;
        case InputKeycode::Key_K:
            return "Key_K";
            break;
        case InputKeycode::Key_L:
            return "Key_L";
            break;
        case InputKeycode::Key_M:
            return "Key_M";
            break;
        case InputKeycode::Key_N:
            return "Key_N";
            break;
        case InputKeycode::Key_O:
            return "Key_O";
            break;
        case InputKeycode::Key_P:
            return "Key_P";
            break;
        case InputKeycode::Key_Q:
            return "Key_Q";
            break;
        case InputKeycode::Key_R:
            return "Key_R";
            break;
        case InputKeycode::Key_S:
            return "Key_S";
            break;
        case InputKeycode::Key_T:
            return "Key_T";
            break;
        case InputKeycode::Key_U:
            return "Key_U";
            break;
        case InputKeycode::Key_V:
            return "Key_V";
            break;
        case InputKeycode::Key_W:
            return "Key_W";
            break;
        case InputKeycode::Key_X:
            return "Key_X";
            break;
        case InputKeycode::Key_Y:
            return "Key_Y";
            break;
        case InputKeycode::Key_Z:
            return "Key_Z";
            break;

            /*************************************************************************
              Keyboard Numbers Layout
            *************************************************************************/
        case InputKeycode::Key_0:
            return "Key_0";
            break;
        case InputKeycode::Key_1:
            return "Key_1";
            break;
        case InputKeycode::Key_2:
            return "Key_2";
            break;
        case InputKeycode::Key_3:
            return "Key_3";
            break;
        case InputKeycode::Key_4:
            return "Key_4";
            break;
        case InputKeycode::Key_5:
            return "Key_5";
            break;
        case InputKeycode::Key_6:
            return "Key_6";
            break;
        case InputKeycode::Key_7:
            return "Key_7";
            break;
        case InputKeycode::Key_8:
            return "Key_8";
            break;
        case InputKeycode::Key_9:
            return "Key_9";
            break;

            /*************************************************************************
              KeyPad Keys Layout ( The one on the right of your keyboard )
            *************************************************************************/
        case InputKeycode::KeyPad_ENTER:
            return "KeyPad Enter";
            break;
        case InputKeycode::KeyPad_EQUAL:
            return "KeyPad Equal";
            break;
        case InputKeycode::KeyPad_MULTIPLY:
            return "KeyPad Multiply";
            break;
        case InputKeycode::KeyPad_ADD:
            return "KeyPad Add";
            break;
        case InputKeycode::KeyPad_SEPERATOR:
            return "KeyPad Seperator";
            break;
        case InputKeycode::KeyPad_SUBTRACT:
            return "KeyPad Subtract";
            break;
        case InputKeycode::KeyPad_DECIMAL:
            return "KeyPad Decimal";
            break;
        case InputKeycode::KeyPad_DIVIDE:
            return "KeyPad Divide";
            break;

            /*************************************************************************
              Keyboard F Keys Layout
            *************************************************************************/
        case InputKeycode::Key_F1:
            return "Key_F1";
            break;
        case InputKeycode::Key_F2:
            return "Key_F2";
            break;
        case InputKeycode::Key_F3:
            return "Key_F3";
            break;
        case InputKeycode::Key_F4:
            return "Key_F4";
            break;
        case InputKeycode::Key_F5:
            return "Key_F5";
            break;
        case InputKeycode::Key_F6:
            return "Key_F6";
            break;
        case InputKeycode::Key_F7:
            return "Key_F7";
            break;
        case InputKeycode::Key_F8:
            return "Key_F8";
            break;
        case InputKeycode::Key_F9:
            return "Key_F9";
            break;
        case InputKeycode::Key_F10:
            return "Key_F10";
            break;
        case InputKeycode::Key_F11:
            return "Key_F11";
            break;
        case InputKeycode::Key_F12:
            return "Key_F12";
            break;

            /*************************************************************************
              Keyboard Arrow Keys Layout
            *************************************************************************/
        case InputKeycode::Key_LEFT:
            return "Key_LEFT";
            break;
        case InputKeycode::Key_UP:
            return "Key_UP";
            break;
        case InputKeycode::Key_RIGHT:
            return "Key_RIGHT";
            break;
        case InputKeycode::Key_DOWN:
            return "Key_DOWN";
            break;

            /*************************************************************************
              Keyboard OTHER Keys Layout
            *************************************************************************/
        case InputKeycode::Key_BACKSPACE:
            return "Key_BACKSPACE";
            break;
        case InputKeycode::Key_TAB:
            return "Key_TAB";
            break;
        case InputKeycode::Key_PAUSE:
            return "Key_PAUSE";
            break;
        case InputKeycode::Key_CAPSLOCK:
            return "Key_CapsLock";
            break;
        case InputKeycode::Key_ESC:
            return "Key_ESC";
            break;
        case InputKeycode::Key_SPACE:
            return "Key_SPACE";
            break;
        case InputKeycode::Key_ENTER:
            return "Key_Enter";
            break;
        case InputKeycode::Key_INSERT:
            return "Key_INSERT";
            break;
        case InputKeycode::Key_DELETE:
            return "Key_DELETE";
            break;
        case InputKeycode::Key_NUMLOCK:
            return "Key_NUMLOCK";
            break;
        case InputKeycode::Key_SHIFTLEFT:
            return "Key_SHIFTLEFT";
            break;
        case InputKeycode::Key_SHIFTRIGHT:
            return "Key_SHIFTRIGHT";
            break;
        case InputKeycode::Key_CTRLLEFT:
            return "Key_CTRLLEFT";
            break;
        case InputKeycode::Key_CTRLRIGHT:
            return "Key_CTRLRIGHT";
            break;
        case InputKeycode::Key_ALTLEFT:
            return "Key_ALTLEFT";
            break;
        case InputKeycode::Key_ALTRIGHT:
            return "Key_ALTRIGHT";
            break;
        case InputKeycode::Key_SEMICOLON:
            return "Key_SEMICOLON";
            break;
        case InputKeycode::Key_COMMA:
            return "Key_COMMA";
            break;
        case InputKeycode::Key_MINUS:
            return "Key_MINUS";
            break;
        case InputKeycode::Key_EQUAL:
            return "Key_EQUAL";
            break;
        case InputKeycode::Key_FORWARDSLASH:
            return "Key_FORWARDSLASH";
            break;
        case InputKeycode::Key_SQBRACKETLEFT:
            return "Key_SQBRACKETLEFT";
            break;
        case InputKeycode::Key_BACKSLASH:
            return "Key_BACKSLASH ";
            break;
        case InputKeycode::Key_SQBRACKETRIGHT:
            return "Key_SQBRACKETRIGHT";
            break;

            // cant work on my stupid ass laptop
        case InputKeycode::Key_CLEAR:
            return "Key_CLEAR";
            break;
        case InputKeycode::Key_RETURN:
            return "Key_RETURN";
            break;
        case InputKeycode::Key_SHIFT:
            return "Key_SHIFT";
            break;
        case InputKeycode::Key_CTRL:
            return "Key_CTRL";
            break;
        case InputKeycode::Key_ALT:
            return "Key_ALT";
            break;
        case InputKeycode::Key_PGUP:
            return "Key_PGUP";
            break;
        case InputKeycode::Key_PGDN:
            return "Key_PGDN";
            break;
        case InputKeycode::Key_END:
            return "Key_END ";
            break;
        case InputKeycode::Key_HOME:
            return "Key_HOME";
            break;
        case InputKeycode::Key_SELECT:
            return "Key_SELECT";
            break;
        case InputKeycode::Key_PRINT:
            return "Key_PRINT";
            break;
        case InputKeycode::Key_EXECUTE:
            return "Key_EXECUTE";
            break;
        case InputKeycode::Key_PRTSCREEN:
            return "Key_PRTSC";
            break;
        case InputKeycode::Key_HELP:
            return "Key_HELP";
            break;
        case InputKeycode::Key_APPS:
            return "Key_APPS";
            break;
        case InputKeycode::Key_SLEEP:
            return "Key_SLEEP";
            break;
        case InputKeycode::Key_MULTIPLY:
            return "Key_MULTIPLY";
            break;
        case InputKeycode::Key_ADD:
            return "Key_ADD";
            break;
        case InputKeycode::Key_SEPERATOR:
            return "Key_SEPERATOR";
            break;
        case InputKeycode::Key_SUBTRACT:
            return "Key_SUBTRACT";
            break;
        case InputKeycode::Key_DECIMAL:
            return "Key_DECIMAL";
            break;
        case InputKeycode::Key_DIVIDE:
            return "Key_DIVIDE";
            break;
        case InputKeycode::Key_SCROLLLOCK:
            return "Key_SCROLLLOCK";
            break;
        case InputKeycode::Key_PLUS:
            return "Key_PLUS";
            break;
        case InputKeycode::Key_QUOTE:
            return "Key_QUOTE";
            break;
        case InputKeycode::Key_MISCELLANEOUS:
            return "Key_MISCELLANEOUS";
            break;
        default:
            return "";
            break;
        }
    }
}

static void on_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    (void)window;
    (void)scancode;
    (void)mods;

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        std::cout << "Move 1" << std::endl;
    }

    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        std::cout << "Move 2" << std::endl;
    }
}

namespace Eclipse
{
    void LogicalInput::SetAxis(GLint width, GLint height)
    {
        XMiddle = static_cast<float>((width / 2));
        YMiddle = static_cast<float>((height / 2));
    }

    void LogicalInput::CursorUpdate()
    {
        //When Locked, the cursor is placed in the center of the viewand cannot be moved.The cursor is invisible in this state, regardless of the value of Cursor.visible.
        //When Confined, the cursor behaves normally with the exception of being confined to the view.For example, if the application is running in a window, the mouse cursor cannot leave the window in Confined mode.This is only supported on Windowsand Linux standalone builds.
        //To provide a good user experience the recommended behavior is only to lock or confine the cursor as a result of user action, for example by pressing a button.

        switch (State)
        {
        case CursorLockMode::Locked:
        {
            if (HideCursor)
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_None);
            }

            auto* scene = engine->editorManager->GetEditorWindow<eGameViewWindow>();

            float GameWindowMinX = scene->vMin.x - OpenGL_Context::GetContextPosition().x;
            float GameWindowMaxX = scene->vMin.x + scene->GetViewPortSize().getX() - OpenGL_Context::GetContextPosition().x;
            float GameWindowCenterX = (GameWindowMaxX + GameWindowMinX) / 2;

            float GameWindowMinY = scene->vMin.y - OpenGL_Context::GetContextPosition().y;
            float GameWindowMaxY = scene->vMin.y + scene->GetViewPortSize().getY() - OpenGL_Context::GetContextPosition().y;
            float GameWindowCenterY = (GameWindowMinY + GameWindowMaxY) / 2;

            glfwSetCursorPos(OpenGL_Context::ptr_window, GameWindowCenterX, GameWindowCenterY);
        }
        break;

        case CursorLockMode::None:
        {
            // Unhides cursor 
            //glfwSetInputMode(OpenGL_Context::ptr_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        break;

        }
    }

    void LogicalInput::AxisUpdate()
    {
        float MouseX = static_cast<float>(OpenGL_Context::m_posX);
        float MouseY = static_cast<float>(OpenGL_Context::m_posY);

        // HORIZONTAL

        // If Mouse position Different from Old One
        if (MouseX != Mouse_X)
        {
            // We checkf if current mouse is bigger
            if (MouseX > Mouse_X)
            {
                // if bigger we +
                if (XDelta <= 1.0f)
                {
                    XDelta += engine->Game_Clock.get_DeltaTime();
                    XDeltaRaw = 1.0f;
                }
            }
            else
            {
                // if not we minus
                if (XDelta >= -1.0f)
                {
                    XDelta -= engine->Game_Clock.get_DeltaTime();
                    XDeltaRaw = -1.0f;
                }
            }

            Mouse_X = MouseX;
            //Mouse_Y = MouseY;
        }
        else
        {
            // If not moving , we reset.
            XDelta = 0;
            XDeltaRaw = 0;
        }

        // VERTICAL
        // We check if mouse y is same as old position or not
        if (MouseY != Mouse_Y)
        {
            // If Mouse Y is bigger than previous
            if (MouseY < Mouse_Y)
            {
                if (YDelta >= -1.0f)
                {
                    YDelta -= engine->Game_Clock.get_DeltaTime();
                    YDeltaRaw = -1.0f;
                }
            }
            else
            {
                if (YDelta <= 1.0f)
                {
                    YDelta += engine->Game_Clock.get_DeltaTime();
                    YDeltaRaw = 1.0f;
                }
            }

            //Mouse_X = MouseX;
            Mouse_Y = MouseY;
        }
        else
        {
            YDelta = 0;
            YDeltaRaw = 0;
        }
    }

    float LogicalInput::GetAxis(const std::string& Type)
    {
        if (Type == "Mouse X")
        {
            return XDelta;
        }
        else if (Type == "Mouse Y")
        {
            return YDelta;
        }
        else if (Type == "Horizontal")
        {
            if (GetKeyCurrent(InputKeycode::Key_A) || GetKeyCurrent(InputKeycode::Key_LEFT))
            {
                if (XDeltaKey > -1.0f)
                {
                    return XDeltaKey -= engine->Game_Clock.get_DeltaTime();
                }
            }
            else if (GetKeyCurrent(InputKeycode::Key_D) || GetKeyCurrent(InputKeycode::Key_RIGHT))
            {
                if (XDeltaKey < 1.0f)
                {
                    return XDeltaKey += engine->Game_Clock.get_DeltaTime();
                }
            }
            else
            {
                if (XDeltaKey > 0.0f)
                    return XDeltaKey -= engine->Game_Clock.get_DeltaTime();
                else if (XDeltaKey < 0.0f)
                    return XDeltaKey += engine->Game_Clock.get_DeltaTime();
                else
                    return (XDeltaKey = 0.0f);
            }
        }
        else if (Type == "Vertical")
        {
            if (GetKeyCurrent(InputKeycode::Key_DOWN) || GetKeyCurrent(InputKeycode::Key_S))
            {
                if (YDeltaKey > -1.0f)
                {
                    return YDeltaKey -= engine->Game_Clock.get_DeltaTime();
                }
            }
            else if (GetKeyCurrent(InputKeycode::Key_UP) || GetKeyCurrent(InputKeycode::Key_W))
            {
                if (YDeltaKey < 1.0f)
                {
                    return YDeltaKey += engine->Game_Clock.get_DeltaTime();
                }
            }
            else
            {
                if (YDeltaKey > 0.0f)
                    return YDeltaKey -= engine->Game_Clock.get_DeltaTime();
                else if (YDeltaKey < 0.0f)
                    return YDeltaKey += engine->Game_Clock.get_DeltaTime();
                else
                    return (YDeltaKey = 0.0f);
            }
        }

        return 0.0f;
    }

    bool LogicalInput::LockCursor(CursorLockMode Mode)
    {
        //When Locked, the cursor is placed in the center of the viewand cannot be moved.The cursor is invisible in this state, regardless of the value of Cursor.visible.
        //When Confined, the cursor behaves normally with the exception of being confined to the view.For example, if the application is running in a window, the mouse cursor cannot leave the window in Confined mode.This is only supported on Windowsand Linux standalone builds.
        //To provide a good user experience the recommended behavior is only to lock or confine the cursor as a result of user action, for example by pressing a button.

        State = Mode;

        return true;
    }
}