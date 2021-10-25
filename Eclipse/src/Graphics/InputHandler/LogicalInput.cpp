#include "pch.h"
#include "LogicalInput.h"
#include "Graphics/OpenGL/OpenGL_Context.h"

static void on_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
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
    LogicalInput::LogicalInput()
    {
        EnablePrint = false;

        init();
        InsertAllKeys();

        KeyMappings.emplace("Horizontal", InputKeycode::Key_LEFT);
    }

    void LogicalInput::InsertAllKeys()
    {
        for (unsigned char i = 0; i <= static_cast<unsigned char>(Eclipse::InputKeycode::Key_MISCELLANEOUS); i++)
        {
            std::string Key = GetInputString(static_cast<Eclipse::InputKeycode>(i)).data();

            if (Key.empty() == false)
            {
                AllInputs.push_back(Key);
            }
        }
    }

    std::vector<std::string> LogicalInput::GetAllKeys()
    {
        return AllInputs;
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

    void LogicalInput::init()
    {
        // Clear Container just in case
        KeyContainer.clear();
        HoldKeyContainer.clear();
    }

    bool LogicalInput::GetIsPrint()
    {
        return EnablePrint;
    }

    void LogicalInput::SetIsPrint(bool input)
    {
        EnablePrint = input;
    }

    void LogicalInput::RegisterTriggerInput(InputKeycode keycode, int keypressFlag, bool flag, InputState input)
    {
        KeyContainer.insert({ keycode,std::pair<bool,InputState>(flag,input) });
    }

    void LogicalInput::RegisterHoldInput(InputKeycode keycode, int keypressFlag, bool flag, InputState input)
    {

        for (KeyIT it = HoldKeyContainer.begin(); it != HoldKeyContainer.end(); it++)
        {
            if (!(it->first == keycode))
            {
                HoldKeyContainer.insert({ keycode,std::pair<bool,InputState>(flag,input) });
            }
        }
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
            std::cout << "Container Size : " << HoldKeyContainer.size() << std::endl;
        }
    }

    void LogicalInput::RemoveKey(KeyIT& input)
    {
        KeyContainer.erase(input);
    }

    void LogicalInput::RemoveHoldKey(KeyIT& input)
    {
        HoldKeyContainer.erase(input);
    }

    void LogicalInput::UpdateKeys()
    {
        for (auto& pair2 : KeyContainer)
        {
            if (pair2.second.second == InputState::Key_TRIGGERED)
            {
                pair2.second.second = InputState::Key_HOLD;
            }
        }
    }

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
            bool single = false;

            RegisterTriggerInput(keycode, Press, single, InputState::Key_TRIGGERED); // container size 1

            if (KeyContainer.count(keycode) != 0 && KeyContainer[keycode].second == InputState::Key_TRIGGERED)
            {
                PrintKey(keycode, Press);
                return true;
            }
            else
            {
                return false;
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
            bool single = false;
            RegisterHoldInput(keycode, Hold, single, InputState::Key_HOLD);

            if (KeyContainer.count(keycode) != 0 &&
                (KeyContainer[keycode].second == InputState::Key_TRIGGERED || KeyContainer[keycode].second == InputState::Key_HOLD))
            {
                //PrintKey(keycode, Hold);
                return true;
            }
            else
            {
                return false;
            }
        }

        return false;
    }

    bool LogicalInput::GetKeyReleased(InputKeycode keycode)
    {
        int Release = GetKey(keycode);

        if (IsKeyReleased(Release))
        {
            if (KeyContainer.count(keycode) != 0)
            {
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

    bool LogicalInput::GetHoldKeyReleased(InputKeycode keycode)
    {
        int HoldKeyRelease = GetKey(keycode);

        if (IsKeyReleased(HoldKeyRelease))
        {
            if (KeyContainer.count(keycode) != 0)
            {
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

    void LogicalInput::Update()
    {
        for (auto& pair2 : HoldKeyContainer)
        {
            if (HoldKeyContainer.size() == 1)
                return;

            auto& Message = (pair2.first);
            auto& HoldFlag = (pair2.second.second);

            if (HoldFlag == InputState::Key_HOLD)
            {
                PrintKey(Message);
            }
        }
    }
}

namespace Eclipse
{
    InputKeycode LogicalInput::CheckMappingExist(std::string NewMap)
    {
        // Searching for element 12
        if (KeyMappings.find(NewMap) == KeyMappings.end())
        {
            // Cannot find
            return InputKeycode::Key_Null;
        }
        else
        {
            // Can Find
            return KeyMappings[NewMap];
        }
    }

    bool LogicalInput::GetKeyTriggered(std::string Mappedkeycode)
    {
        int Press = GetKey(CheckMappingExist(Mappedkeycode));

        if (IsKeyReleased(Press))
        {
            GetKeyReleased(CheckMappingExist(Mappedkeycode));
            return false;
        }
        // Check if Space is pressed
        else if (IsKeyPressed(Press))
        {
            bool single = false;

            RegisterTriggerInput(CheckMappingExist(Mappedkeycode), Press, single, InputState::Key_TRIGGERED); // container size 1

            if (KeyContainer.count(CheckMappingExist(Mappedkeycode)) != 0 && KeyContainer[CheckMappingExist(Mappedkeycode)].second == InputState::Key_TRIGGERED)
            {
                PrintKey(CheckMappingExist(Mappedkeycode), Press);
                return true;
            }
            else
            {
                return false;
            }
        }

        return false;
    }

    bool LogicalInput::GetKeyCurrent(std::string Mappedkeycode)
    {
        int Hold = GetKey(CheckMappingExist(Mappedkeycode));

        if (IsKeyReleased(Hold))
        {
            GetHoldKeyReleased(CheckMappingExist(Mappedkeycode));
            return false;
        }
        else if (IsKeyPressed(Hold))
        {
            bool single = false;
            RegisterHoldInput(CheckMappingExist(Mappedkeycode), Hold, single, InputState::Key_HOLD);

            if (KeyContainer.count(CheckMappingExist(Mappedkeycode)) != 0 &&
                (KeyContainer[CheckMappingExist(Mappedkeycode)].second == InputState::Key_TRIGGERED || KeyContainer[CheckMappingExist(Mappedkeycode)].second == InputState::Key_HOLD))
            {
                //PrintKey(keycode, Hold);
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