#pragma once

namespace Eclipse
{
	enum class InputKeycode : unsigned char
	{
		KeyCode_NULL = 0,

		KeyCode_CANCEL = 3,				//CTRL + BREAK
		KeyCode_BACKSPACE = 8,
		KeyCode_TAB = 9,
		KeyCode_CLEAR = 12,
		KeyCode_RETURN = 13,
		KeyCode_SHIFT = 16,
		KeyCode_CTRL = 17,
		KeyCode_ALT = 18,
		KeyCode_PAUSE = 19,
		KeyCode_CAPSLOCK = 20,
		KeyCode_ESC = 27,
		KeyCode_SPACE = 32,
		KeyCode_PGUP = 33,
		KeyCode_PGDN = 34,
		KeyCode_END = 35,
		KeyCode_HOME = 36,

		KeyCode_LEFT = 37,
		KeyCode_UP = 38,
		KeyCode_RIGHT = 39,
		KeyCode_DOWN = 40,
		KeyCode_SELECT = 41,
		KeyCode_PRINT = 42,
		KeyCode_EXECUTE = 43,
		KeyCode_PRTSC = 44,			//printscreen
		KeyCode_INSERT = 45,
		KeyCode_DELETE = 46,
		KeyCode_HELP = 47,

		KeyCode_0 = 48,
		KeyCode_1 = 49,
		KeyCode_2 = 50,
		KeyCode_3 = 51,
		KeyCode_4 = 52,
		KeyCode_5 = 53,
		KeyCode_6 = 54,
		KeyCode_7 = 55,
		KeyCode_8 = 56,
		KeyCode_9 = 57,

		KeyCode_A = 65,
		KeyCode_B = 66,
		KeyCode_C = 67,
		KeyCode_D = 68,
		KeyCode_E = 69,
		KeyCode_F = 70,
		KeyCode_G = 71,
		KeyCode_H = 72,
		KeyCode_I = 73,
		KeyCode_J = 74,
		KeyCode_K = 75,
		KeyCode_L = 76,
		KeyCode_M = 77,
		KeyCode_N = 78,
		KeyCode_O = 79,
		KeyCode_P = 80,
		KeyCode_Q = 81,
		KeyCode_R = 82,
		KeyCode_S = 83,
		KeyCode_T = 84,
		KeyCode_U = 85,
		KeyCode_V = 86,
		KeyCode_W = 87,
		KeyCode_X = 88,
		KeyCode_Y = 89,
		KeyCode_Z = 90,

		KeyCode_WINLEFT = 91,
		KeyCode_WINRIGHT = 92,
		KeyCode_APPS = 93,
		KeyCode_SLEEP = 95,

		KeyCode_NUM0 = 96,
		KeyCode_NUM1 = 97,
		KeyCode_NUM2 = 98,
		KeyCode_NUM3 = 99,
		KeyCode_NUM4 = 100,
		KeyCode_NUM5 = 101,
		KeyCode_NUM6 = 102,
		KeyCode_NUM7 = 103,
		KeyCode_NUM8 = 104,
		KeyCode_NUM9 = 105,

		KeyCode_MULTIPLY = 106,
		KeyCode_ADD = 107,
		KeyCode_SEPERATOR = 108,
		KeyCode_SUBTRACT = 109,
		KeyCode_DECIMAL = 110,
		KeyCode_DIVIDE = 111,

		KeyCode_F1 = 112,
		KeyCode_F2 = 113,
		KeyCode_F3 = 114,
		KeyCode_F4 = 115,
		KeyCode_F5 = 116,
		KeyCode_F6 = 117,
		KeyCode_F7 = 118,
		KeyCode_F8 = 119,
		KeyCode_F9 = 120,
		KeyCode_F10 = 121,
		KeyCode_F11 = 122,
		KeyCode_F12 = 123,			//Until F24...

		KeyCode_NUMLOCK = 144,
		KeyCode_SCROLLLOCK = 145,

		KeyCode_SHIFTLEFT = 160,
		KeyCode_SHIFTRIGHT = 161,
		KeyCode_CTRLLEFT = 162,
		KeyCode_CTRLRIGHT = 163,
		KeyCode_ALTLEFT = 164,
		KeyCode_ALTRIGHT = 165,

		KeyCode_SEMICOLON = 186,			//OEM 1
		KeyCode_PLUS = 187,
		KeyCode_COMMA = 188,
		KeyCode_MINUS = 189,
		KeyCode_PERIOD = 190,
		KeyCode_FORWARDSLASH = 191,			//OEM 2 '/' '?'
		KeyCode_TILDE = 192,				//OEM 3			
		KeyCode_SQBRACKETLEFT = 193,
		KeyCode_BACKSLASH = 194,
		KeyCode_SQBRACKETRIGHT = 195,
		KeyCode_QUOTE = 196,
		KeyCode_MISCELLANEOUS = 197,		//OEM 8
	};

	enum class InputMouseKeycode : unsigned char
	{
		KeyCode_MOUSENULL = 0,
		KeyCode_MOUSELEFT = 1,
		KeyCode_MOUSERIGHT = 2,
		KeyCode_MOUSEMIDDLE = 4,
		KeyCode_MOUSEX1 = 5,			//The additional side key of the mouse
		KeyCode_MOUSEX2 = 6,			//The additional side key of the mouse
	};

	enum class InputState : unsigned char
	{
		Key_NULLSTATE,
		Key_RELEASED,
		Key_PRESSED,
		Key_HELD
	};
}
