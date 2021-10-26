#pragma once

namespace Eclipse
{
    //https://www.oreilly.com/library/view/javascript-dhtml/9780596514082/apb.html
    //https://www.glfw.org/docs/3.3/group__keys.html
    enum class InputState
    {
        Key_NULLSTATE,
        Key_RELEASED,
        Key_PRESSED,
        Key_TRIGGERED,
        Key_HOLD
    };

    enum class InputMouseKeycode : unsigned int
    {
        KeyCode_NONE = 2,
        KeyCode_MOUSELEFT = 0,
        KeyCode_MOUSERIGHT = 1
    };

    enum class InputKeycode : unsigned int
    {
        Key_Null = 0,

        /*************************************************************************
          Keyboard Alphabets Layout
        *************************************************************************/
        Key_A = 65,
        Key_B = 66,
        Key_C = 67,
        Key_D = 68,
        Key_E = 69,
        Key_F = 70,
        Key_G = 71,
        Key_H = 72,
        Key_I = 73,
        Key_J = 74,
        Key_K = 75,
        Key_L = 76,
        Key_M = 77,
        Key_N = 78,
        Key_O = 79,
        Key_P = 80,
        Key_Q = 81,
        Key_R = 82,
        Key_S = 83,
        Key_T = 84,
        Key_U = 85,
        Key_V = 86,
        Key_W = 87,
        Key_X = 88,
        Key_Y = 89,
        Key_Z = 90,

        /*************************************************************************
          Keyboard Numbers Layout
        *************************************************************************/
        Key_0 = 48,
        Key_1 = 49,
        Key_2 = 50,
        Key_3 = 51,
        Key_4 = 52,
        Key_5 = 53,
        Key_6 = 54,
        Key_7 = 55,
        Key_8 = 56,
        Key_9 = 57,

        /*************************************************************************
          KeyPad Keys Layout ( The one on the right of your keyboard )
        *************************************************************************/
        KeyPad_ENTER = 335,
        KeyPad_EQUAL = 336,
        KeyPad_MULTIPLY = 332,
        KeyPad_ADD = 334,
        KeyPad_SEPERATOR = 108,
        KeyPad_SUBTRACT = 333,
        KeyPad_DECIMAL = 330,
        KeyPad_DIVIDE = 331,

        /*************************************************************************
          Keyboard F Keys Layout
        *************************************************************************/
        Key_F1 = 290,
        Key_F2 = 291,
        Key_F3 = 292,
        Key_F4 = 293,
        Key_F5 = 294,
        Key_F6 = 295,
        Key_F7 = 296,
        Key_F8 = 297,
        Key_F9 = 298,
        Key_F10 = 299,
        Key_F11 = 300,
        Key_F12 = 301,

        /*************************************************************************
          Keyboard Arrow Keys Layout
        *************************************************************************/
        Key_LEFT = 263,
        Key_UP = 265,
        Key_RIGHT = 262,
        Key_DOWN = 264,

        /*************************************************************************
          Keyboard OTHER Keys Layout
        *************************************************************************/
        Key_BACKSPACE = 259,
        Key_TAB = 258,
        Key_PAUSE = 284,
        Key_CAPSLOCK = 280,
        Key_ESC = 256,
        Key_SPACE = 32,
        Key_ENTER = 257,
        Key_INSERT = 260,
        Key_DELETE = 261,
        Key_NUMLOCK = 282,
        Key_SHIFTLEFT = 340,
        Key_SHIFTRIGHT = 344,
        Key_CTRLLEFT = 341,
        Key_CTRLRIGHT = 345,
        Key_ALTLEFT = 342,
        Key_ALTRIGHT = 346,
        Key_SEMICOLON = 59,
        Key_COMMA = 44,
        Key_MINUS = 45,
        Key_EQUAL = 61,
        Key_FORWARDSLASH = 47,
        Key_SQBRACKETLEFT = 91,
        Key_BACKSLASH = 92,
        Key_SQBRACKETRIGHT = 93,

        // cant work on my stupid ass laptop
        Key_MULTIPLY = 106,
        Key_ADD = 107,
        Key_SEPERATOR = 1081,
        Key_SUBTRACT = 109,
        Key_DECIMAL = 110,
        Key_DIVIDE = 111,
        Key_CANCEL = 3,
        Key_CLEAR = 12,
        Key_RETURN = 13,
        Key_SHIFT = 16,
        Key_CTRL = 17,
        Key_ALT = 18,
        Key_PGUP = 266,
        Key_PGDN = 267,
        Key_END = 269,
        Key_HOME = 268,
        Key_SELECT = 41,
        Key_PRINT = 42,
        Key_EXECUTE = 43,
        Key_HELP = 471,
        Key_PRTSCREEN = 283,
        Key_APPS = 931,
        Key_SLEEP = 95,
        Key_SCROLLLOCK = 281, // cann
        Key_PLUS = 187,
        Key_PERIOD = 46,
        Key_TILDE = 192,
        Key_QUOTE = 196,
        Key_MISCELLANEOUS = 197,
    };
}