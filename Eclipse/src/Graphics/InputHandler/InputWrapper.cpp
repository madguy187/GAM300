#include "pch.h"
#include "InputWrapper.h"
#include "Graphics/OpenGL/OpenGL_Context.h"

namespace Eclipse
{

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

    InputWrapper::InputWrapper() :
        EnablePrint{ false }
    {
        init();
    }

    void InputWrapper::init()
    {

        // Clear Container just in case
        KeyContainer.clear();
        HoldKeyContainer.clear();

        // Insert one inactive key into container for Hold Key Container , no need for trigger container
        HoldKeyContainer.insert({ std::pair<InputKeycode,int>(InputKeycode::Key_Null,0),std::pair<bool,InputState>(false,InputState::Key_NULLSTATE) });
    }

    bool InputWrapper::GetIsPrint()
    {
        return EnablePrint;
    }

    void InputWrapper::SetIsPrint(bool input)
    {
        EnablePrint = input;
    }

    void InputWrapper::RegisterTriggerInput(InputKeycode keycode, int keypressFlag, bool flag, InputState input)
    {
        KeyContainer.insert({ std::pair<InputKeycode,int>(keycode,keypressFlag),std::pair<bool,InputState>(flag,input) });
    }

    void InputWrapper::RegisterHoldInput(InputKeycode keycode, int keypressFlag, bool flag, InputState input)
    {

        for (KeyIT it = HoldKeyContainer.begin(); it != HoldKeyContainer.end(); it++)
        {
            if (!(it->first.first == keycode))
            {
                HoldKeyContainer.insert({ std::pair<InputKeycode,int>(keycode,keypressFlag),std::pair<bool,InputState>(flag,input) });
            }
        }
    }

    bool InputWrapper::IsKeyPressed(int input)
    {
        return (input == GLFW_PRESS);
    }

    bool InputWrapper::IsKeyReleased(int input)
    {
        return (input == GLFW_RELEASE);
    }

    int InputWrapper::GetKey(InputKeycode input)
    {
        return (glfwGetKey(OpenGL_Context::GetWindow(), static_cast<int>(input)));
    }

    void InputWrapper::PrintKey(InputKeycode input, int presstype)
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

    void InputWrapper::PrintKey(InputKeycode input)
    {
        if (EnablePrint)
        {
            std::cout << "Holding " << (input) << " --> " << "KeyCode " << static_cast<int>(input) << std::endl;
            std::cout << "Container Size : " << HoldKeyContainer.size() << std::endl;
        }
    }

    void InputWrapper::RemoveKey(KeyIT& input)
    {
        KeyContainer.erase(input);
    }

    void InputWrapper::RemoveHoldKey(KeyIT& input)
    {
        HoldKeyContainer.erase(input);
    }

    bool InputWrapper::GetKeyTriggered(InputKeycode keycode)
    {
#ifndef CURRENT_CODE

        int Press = GetKey(keycode);

        if (IsKeyPressed(Press))
        {
            bool single = false;

            RegisterTriggerInput(keycode, Press, single, InputState::Key_PRESSED);

            for (auto& pair2 : KeyContainer)
            {
                auto& Message = (pair2.first.first);
                auto& SingleFlag = (pair2.second.first);

                if (SingleFlag)
                    continue;

                SingleFlag = true;
                PrintKey(Message, Press);

                return true;
            }
        }
        else if (IsKeyReleased(Press))
        {
            GetKeyReleased(keycode);
            return false;
        }

        return false;

#else
        int isKeyPressed = glfwGetKey(GLHelper::ptr_window, static_cast<int>(keycode));

        if (isKeyPressed == GLFW_PRESS && !single)
        {
            if (single)
                return false;

            single = true;
            //std::cout << " Create Apple " << std::endl;
            return true;
        }
        else if (isKeyPressed == GLFW_RELEASE)
        {
            single = false;
            return false;
        }
        return false;
#endif

    }

    bool InputWrapper::GetKeyCurrent(InputKeycode keycode)
    {
#ifndef CURRENT_CODE

        Update();

        int Hold = GetKey(keycode);

        if (IsKeyPressed(Hold))
        {
            bool single = false;
            RegisterHoldInput(keycode, Hold, single, InputState::Key_HOLD);
        }
        else if (IsKeyReleased(Hold))
        {
            GetHoldKeyReleased(keycode);
            return false;
        }

        return false;
#else
        return false;
#endif
    }

    bool InputWrapper::GetKeyReleased(InputKeycode keycode)
    {
        int Release = GetKey(keycode);

        if (IsKeyReleased(Release))
        {
            for (auto KeyIT = KeyContainer.begin(); KeyIT != KeyContainer.end(); ++KeyIT)
            {
                auto& Message = ((*KeyIT).first.first);
                auto& InputState_ = ((*KeyIT).second.second);

                if (Message == keycode && (InputState_ == InputState::Key_PRESSED))
                {
                    auto& SingleFlag = ((*KeyIT).second.first);

                    InputState_ = InputState::Key_RELEASED;
                    SingleFlag = false;

                    RemoveKey(KeyIT);
                    PrintKey(Message, Release);

                    return true;
                }
            }
        }
        return false;
    }

    bool InputWrapper::GetHoldKeyReleased(InputKeycode keycode)
    {
        int HoldKeyRelease = GetKey(keycode);

        if (IsKeyReleased(HoldKeyRelease))
        {
            for (auto KeyIT = HoldKeyContainer.begin(); KeyIT != HoldKeyContainer.end(); ++KeyIT)
            {
                auto& Message = ((*KeyIT).first.first);
                auto& InputState_ = ((*KeyIT).second.second);

                if (Message == keycode)
                {
                    auto& SingleFlag = ((*KeyIT).second.first);

                    InputState_ = InputState::Key_RELEASED;
                    SingleFlag = false;

                    RemoveHoldKey(KeyIT);
                    //std::cout << "HOLD CONTAINER SIZE : " << HoldKeyContainer.size() << std::endl;
                    PrintKey(Message, HoldKeyRelease);
                    return true;
                }
            }
        }
        return false;
    }

    void InputWrapper::Update()
    {
        for (auto& pair2 : HoldKeyContainer)
        {
            if (HoldKeyContainer.size() == 1)
                return;

            auto& Message = (pair2.first.first);
            auto& HoldFlag = (pair2.second.second);

            if (HoldFlag == InputState::Key_HOLD)
            {
                PrintKey(Message);
            }
        }
    }

    void Eclipse::InputWrapper::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        (void)window;
        (void)key;
        (void)scancode;
        (void)action;
        (void)mods;
    }
}