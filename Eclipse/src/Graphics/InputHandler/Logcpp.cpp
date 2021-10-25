#include "pch.h"
#include "Graphics/InputHandler/Log.h"
namespace Eclipse
{
    std::ostream&
        operator <<(std::ostream& stream, const Eclipse::InputKeycode& keycode)
    {
        switch (keycode)
        {
        case Eclipse::InputKeycode::KeyCode_NULL: stream << "KeyCode_NULL";				break;
            break;
        case Eclipse::InputKeycode::KeyCode_CANCEL: stream << "KeyCode_CANCEL";			break;
        case Eclipse::InputKeycode::KeyCode_BACKSPACE: stream << "KeyCode_BACKSPACE";			break;
        case Eclipse::InputKeycode::KeyCode_TAB: stream << "KeyCode_TAB";				break;
        case Eclipse::InputKeycode::KeyCode_CLEAR: stream << "KeyCode_CLEAR";				break;
        case Eclipse::InputKeycode::KeyCode_RETURN: stream << "KeyCode_RETURN";			break;
        case Eclipse::InputKeycode::KeyCode_SHIFT: stream << "KeyCode_SHIFT";				break;
        case Eclipse::InputKeycode::KeyCode_CTRL: stream << "KeyCode_CTRL";				break;
        case Eclipse::InputKeycode::KeyCode_ALT: stream << "KeyCode_ALT";				break;
        case Eclipse::InputKeycode::KeyCode_PAUSE: stream << "KeyCode_PAUSE";				break;
        case Eclipse::InputKeycode::KeyCode_CAPSLOCK: stream << "KeyCode_CAPSLOCK";			break;
        case Eclipse::InputKeycode::KeyCode_ESC: stream << "KeyCode_ESC";				break;
        case Eclipse::InputKeycode::KeyCode_SPACE: stream << "KeyCode_SPACE";				break;
        case Eclipse::InputKeycode::KeyCode_PGUP: stream << "KeyCode_PGUP";				break;
        case Eclipse::InputKeycode::KeyCode_PGDN: stream << "KeyCode_PGDN";				break;
        case Eclipse::InputKeycode::KeyCode_END: stream << "KeyCode_END ";				break;
        case Eclipse::InputKeycode::KeyCode_HOME: stream << "KeyCode_HOME";				break;
            break;
        case Eclipse::InputKeycode::KeyCode_LEFT: stream << "KeyCode_LEFT";				break;
        case Eclipse::InputKeycode::KeyCode_UP: stream << "KeyCode_UP";				break;
        case Eclipse::InputKeycode::KeyCode_RIGHT: stream << "KeyCode_RIGHT";				break;
        case Eclipse::InputKeycode::KeyCode_DOWN: stream << "KeyCode_DOWN";				break;
        case Eclipse::InputKeycode::KeyCode_SELECT: stream << "KeyCode_SELECT";			break;
        case Eclipse::InputKeycode::KeyCode_PRINT: stream << "KeyCode_PRINT";				break;
        case Eclipse::InputKeycode::KeyCode_EXECUTE: stream << "KeyCode_EXECUTE";			break;
        case Eclipse::InputKeycode::KeyCode_PRTSC: stream << "KeyCode_PRTSC";				break;
        case Eclipse::InputKeycode::KeyCode_INSERT: stream << "KeyCode_INSERT";			break;
        case Eclipse::InputKeycode::KeyCode_DELETE: stream << "KeyCode_DELETE";			break;
        case Eclipse::InputKeycode::KeyCode_HELP: stream << "KeyCode_HELP";				break;
            break;
        case Eclipse::InputKeycode::KeyCode_0: stream << "KeyCode_0";					break;
        case Eclipse::InputKeycode::KeyCode_1: stream << "KeyCode_1";					break;
        case Eclipse::InputKeycode::KeyCode_2: stream << "KeyCode_2";					break;
        case Eclipse::InputKeycode::KeyCode_3: stream << "KeyCode_3";					break;
        case Eclipse::InputKeycode::KeyCode_4: stream << "KeyCode_4";					break;
        case Eclipse::InputKeycode::KeyCode_5: stream << "KeyCode_5";					break;
        case Eclipse::InputKeycode::KeyCode_6: stream << "KeyCode_6";					break;
        case Eclipse::InputKeycode::KeyCode_7: stream << "KeyCode_7";					break;
        case Eclipse::InputKeycode::KeyCode_8: stream << "KeyCode_8";					break;
        case Eclipse::InputKeycode::KeyCode_9: stream << "KeyCode_9";					break;
            break;
        case Eclipse::InputKeycode::KeyCode_A: stream << "KeyCode_A";					break;
        case Eclipse::InputKeycode::KeyCode_B: stream << "KeyCode_B";					break;
        case Eclipse::InputKeycode::KeyCode_C: stream << "KeyCode_C";					break;
        case Eclipse::InputKeycode::KeyCode_D: stream << "KeyCode_D";					break;
        case Eclipse::InputKeycode::KeyCode_E: stream << "KeyCode_E";					break;
        case Eclipse::InputKeycode::KeyCode_F: stream << "KeyCode_F";					break;
        case Eclipse::InputKeycode::KeyCode_G: stream << "KeyCode_G";					break;
        case Eclipse::InputKeycode::KeyCode_H: stream << "KeyCode_H";					break;
        case Eclipse::InputKeycode::KeyCode_I: stream << "KeyCode_I";					break;
        case Eclipse::InputKeycode::KeyCode_J: stream << "KeyCode_J";					break;
        case Eclipse::InputKeycode::KeyCode_K: stream << "KeyCode_K";					break;
        case Eclipse::InputKeycode::KeyCode_L: stream << "KeyCode_L";					break;
        case Eclipse::InputKeycode::KeyCode_M: stream << "KeyCode_M";					break;
        case Eclipse::InputKeycode::KeyCode_N: stream << "KeyCode_N";					break;
        case Eclipse::InputKeycode::KeyCode_O: stream << "KeyCode_O";					break;
        case Eclipse::InputKeycode::KeyCode_P: stream << "KeyCode_P";					break;
        case Eclipse::InputKeycode::KeyCode_Q: stream << "KeyCode_Q";					break;
        case Eclipse::InputKeycode::KeyCode_R: stream << "KeyCode_R";					break;
        case Eclipse::InputKeycode::KeyCode_S: stream << "KeyCode_S";					break;
        case Eclipse::InputKeycode::KeyCode_T: stream << "KeyCode_T";					break;
        case Eclipse::InputKeycode::KeyCode_U: stream << "KeyCode_U";					break;
        case Eclipse::InputKeycode::KeyCode_V: stream << "KeyCode_V";					break;
        case Eclipse::InputKeycode::KeyCode_W: stream << "KeyCode_W";					break;
        case Eclipse::InputKeycode::KeyCode_X: stream << "KeyCode_X";					break;
        case Eclipse::InputKeycode::KeyCode_Y: stream << "KeyCode_Y";					break;
        case Eclipse::InputKeycode::KeyCode_Z: stream << "KeyCode_Z";					break;
            break;
        case Eclipse::InputKeycode::KeyCode_WINLEFT: stream << "KeyCode_WINLEFT";			break;
        case Eclipse::InputKeycode::KeyCode_WINRIGHT: stream << "KeyCode_WINRIGHT";			break;
        case Eclipse::InputKeycode::KeyCode_APPS: stream << "KeyCode_APPS";				break;
        case Eclipse::InputKeycode::KeyCode_SLEEP: stream << "KeyCode_SLEEP";				break;
            break;
        case Eclipse::InputKeycode::KeyCode_NUM0: stream << "KeyCode_NUM0";				break;
        case Eclipse::InputKeycode::KeyCode_NUM1: stream << "KeyCode_NUM1";				break;
        case Eclipse::InputKeycode::KeyCode_NUM2: stream << "KeyCode_NUM2";				break;
        case Eclipse::InputKeycode::KeyCode_NUM3: stream << "KeyCode_NUM3";				break;
        case Eclipse::InputKeycode::KeyCode_NUM4: stream << "KeyCode_NUM4";				break;
        case Eclipse::InputKeycode::KeyCode_NUM5: stream << "KeyCode_NUM5";				break;
        case Eclipse::InputKeycode::KeyCode_NUM6: stream << "KeyCode_NUM6";				break;
        case Eclipse::InputKeycode::KeyCode_NUM7: stream << "KeyCode_NUM7";				break;
        case Eclipse::InputKeycode::KeyCode_NUM8: stream << "KeyCode_NUM8";				break;
        case Eclipse::InputKeycode::KeyCode_NUM9: stream << "KeyCode_NUM9";				break;
            break;
        case Eclipse::InputKeycode::KeyCode_MULTIPLY: stream << "KeyCode_MULTIPLY";			break;
        case Eclipse::InputKeycode::KeyCode_ADD: stream << "KeyCode_ADD";				break;
        case Eclipse::InputKeycode::KeyCode_SEPERATOR: stream << "KeyCode_SEPERATOR";			break;
        case Eclipse::InputKeycode::KeyCode_SUBTRACT: stream << "KeyCode_SUBTRACT";			break;
        case Eclipse::InputKeycode::KeyCode_DECIMAL: stream << "KeyCode_DECIMAL";			break;
        case Eclipse::InputKeycode::KeyCode_DIVIDE: stream << "KeyCode_DIVIDE";			break;
            break;
        case Eclipse::InputKeycode::KeyCode_F1: stream << "KeyCode_F1";				break;
        case Eclipse::InputKeycode::KeyCode_F2: stream << "KeyCode_F2";				break;
        case Eclipse::InputKeycode::KeyCode_F3: stream << "KeyCode_F3";				break;
        case Eclipse::InputKeycode::KeyCode_F4: stream << "KeyCode_F4";				break;
        case Eclipse::InputKeycode::KeyCode_F5: stream << "KeyCode_F5";				break;
        case Eclipse::InputKeycode::KeyCode_F6: stream << "KeyCode_F6";				break;
        case Eclipse::InputKeycode::KeyCode_F7: stream << "KeyCode_F7";				break;
        case Eclipse::InputKeycode::KeyCode_F8: stream << "KeyCode_F8";				break;
        case Eclipse::InputKeycode::KeyCode_F9: stream << "KeyCode_F9";				break;
        case Eclipse::InputKeycode::KeyCode_F10: stream << "KeyCode_F10";				break;
        case Eclipse::InputKeycode::KeyCode_F11: stream << "KeyCode_F11";				break;
        case Eclipse::InputKeycode::KeyCode_F12: stream << "KeyCode_F12";				break;

        case Eclipse::InputKeycode::KeyCode_NUMLOCK: stream << "KeyCode_NUMLOCK";  break;
        case Eclipse::InputKeycode::KeyCode_SCROLLLOCK: stream << "KeyCode_SCROLLLOCK";  break;
            break;
        case Eclipse::InputKeycode::KeyCode_SHIFTLEFT: stream << "KeyCode_SHIFTLEFT";  break;
        case Eclipse::InputKeycode::KeyCode_SHIFTRIGHT: stream << "KeyCode_SHIFTRIGHT";  break;
        case Eclipse::InputKeycode::KeyCode_CTRLLEFT: stream << "KeyCode_CTRLLEFT";  break;
        case Eclipse::InputKeycode::KeyCode_CTRLRIGHT: stream << "KeyCode_CTRLRIGHT";  break;
        case Eclipse::InputKeycode::KeyCode_ALTLEFT: stream << "KeyCode_ALTLEFT";  break;
        case Eclipse::InputKeycode::KeyCode_ALTRIGHT: stream << "KeyCode_ALTRIGHT";  break;
            break;
        case Eclipse::InputKeycode::KeyCode_SEMICOLON: stream << "KeyCode_SEMICOLON";   break;
        case Eclipse::InputKeycode::KeyCode_PLUS: stream << "KeyCode_PLUS";  break;
        case Eclipse::InputKeycode::KeyCode_COMMA: stream << "KeyCode_COMMA";  break;
        case Eclipse::InputKeycode::KeyCode_MINUS: stream << "KeyCode_MINUS";  break;
        case Eclipse::InputKeycode::KeyCode_PERIOD: stream << "KeyCode_PERIOD";  break;
        case Eclipse::InputKeycode::KeyCode_FORWARDSLASH: stream << "KeyCode_FORWARDSLASH";  break;
        case Eclipse::InputKeycode::KeyCode_TILDE: stream << "KeyCode_TILDE";  break;
        case Eclipse::InputKeycode::KeyCode_SQBRACKETLEFT: stream << "KeyCode_SQBRACKETLEFT";  break;
        case Eclipse::InputKeycode::KeyCode_BACKSLASH: stream << "KeyCode_BACKSLASH ";  break;
        case Eclipse::InputKeycode::KeyCode_SQBRACKETRIGHT: stream << "KeyCode_SQBRACKETRIGHT";  break;
        case Eclipse::InputKeycode::KeyCode_QUOTE: stream << "KeyCode_QUOTE";  break;
        case Eclipse::InputKeycode::KeyCode_MISCELLANEOUS: stream << "KeyCode_MISCELLANEOUS";  break;
        default:	break;
        }

        return stream;
    }

    std::ostream&
        operator <<(std::ostream& stream, const Eclipse::InputMouseKeycode& keycode)
    {
        switch (keycode)
        {
        case Eclipse::InputMouseKeycode::KeyCode_MOUSENULL:	stream << "KeyCode_MOUSENULL";		 break;
        case Eclipse::InputMouseKeycode::KeyCode_MOUSELEFT:	stream << "KeyCode_MOUSELEFT";		 break;
        case Eclipse::InputMouseKeycode::KeyCode_MOUSERIGHT:	stream << "KeyCode_MOUSERIGHT";		 break;
        case Eclipse::InputMouseKeycode::KeyCode_MOUSEMIDDLE:	stream << "KeyCode_MOUSEMIDDLE";	 break;
        case Eclipse::InputMouseKeycode::KeyCode_MOUSEX1:	stream << "KeyCode_MOUSEX1";		 break;
        case Eclipse::InputMouseKeycode::KeyCode_MOUSEX2:	stream << "KeyCode_MOUSEX2";		 break;
        }

        return stream;
    }
}