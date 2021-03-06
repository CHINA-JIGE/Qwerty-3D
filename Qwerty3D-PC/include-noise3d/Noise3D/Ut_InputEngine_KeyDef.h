#pragma once

namespace Noise3D
{
	namespace Ut
	{
		enum NOISE_MOUSEBUTTON
		{
			NOISE_MOUSEBUTTON_LEFT = 0,
			NOISE_MOUSEBUTTON_MIDDLE = 1,
			NOISE_MOUSEBUTTON_RIGHT = 2,
		};

		enum NOISE_KEY
		{
			NOISE_KEY_0 = DIK_0,
			NOISE_KEY_1 = DIK_1,
			NOISE_KEY_2 = DIK_2,
			NOISE_KEY_3 = DIK_3,
			NOISE_KEY_4 = DIK_4,
			NOISE_KEY_5 = DIK_5,
			NOISE_KEY_6 = DIK_6,
			NOISE_KEY_7 = DIK_7,
			NOISE_KEY_8 = DIK_8,
			NOISE_KEY_9 = DIK_9,
			NOISE_KEY_A = DIK_A,
			NOISE_KEY_ABNT_C1 = DIK_ABNT_C1,
			NOISE_KEY_ABNT_C2 = DIK_ABNT_C2,
			NOISE_KEY_ADD = DIK_ADD,
			NOISE_KEY_APOSTROPHE = DIK_APOSTROPHE,
			NOISE_KEY_APPS = DIK_APPS,
			NOISE_KEY_AT = DIK_AT,
			NOISE_KEY_AX = DIK_AX,
			NOISE_KEY_B = DIK_B,
			NOISE_KEY_BACK = DIK_BACK,
			NOISE_KEY_BACKSLASH = DIK_BACKSLASH,
			NOISE_KEY_C = DIK_C,
			NOISE_KEY_CALCULATOR = DIK_CALCULATOR,
			NOISE_KEY_CAPITAL = DIK_CAPITAL,
			NOISE_KEY_COLON = DIK_COLON,
			NOISE_KEY_COMMA = DIK_COMMA,
			NOISE_KEY_CONVERT = DIK_CONVERT,
			NOISE_KEY_D = DIK_D,
			NOISE_KEY_DECIMAL = DIK_DECIMAL,
			NOISE_KEY_DELETE = DIK_DELETE,
			NOISE_KEY_DIVIDE = DIK_DIVIDE,
			NOISE_KEY_DOWN = DIK_DOWN,
			NOISE_KEY_E = DIK_E,
			NOISE_KEY_END = DIK_END,
			NOISE_KEY_EQUALS = DIK_EQUALS,
			NOISE_KEY_ESCAPE = DIK_ESCAPE,
			NOISE_KEY_F = DIK_F,
			NOISE_KEY_F1 = DIK_F1,
			NOISE_KEY_F2 = DIK_F2,
			NOISE_KEY_F3 = DIK_F3,
			NOISE_KEY_F4 = DIK_F4,
			NOISE_KEY_F5 = DIK_F5,
			NOISE_KEY_F6 = DIK_F6,
			NOISE_KEY_F7 = DIK_F7,
			NOISE_KEY_F8 = DIK_F8,
			NOISE_KEY_F9 = DIK_F9,
			NOISE_KEY_F10 = DIK_F10,
			NOISE_KEY_F11 = DIK_F11,
			NOISE_KEY_F12 = DIK_F12,
			NOISE_KEY_F13 = DIK_F13,
			NOISE_KEY_F14 = DIK_F14,
			NOISE_KEY_F15 = DIK_F15,
			NOISE_KEY_G = DIK_G,
			NOISE_KEY_GRAVE = DIK_GRAVE,
			NOISE_KEY_H = DIK_H,
			NOISE_KEY_HOME = DIK_HOME,
			NOISE_KEY_I = DIK_I,
			NOISE_KEY_INSERT = DIK_INSERT,
			NOISE_KEY_J = DIK_J,
			NOISE_KEY_K = DIK_K,
			NOISE_KEY_KANA = DIK_KANA,
			NOISE_KEY_KANJI = DIK_KANJI,
			NOISE_KEY_L = DIK_L,
			NOISE_KEY_LBRACKET = DIK_LBRACKET,
			NOISE_KEY_LCONTROL = DIK_LCONTROL,
			NOISE_KEY_LEFT = DIK_LEFT,
			NOISE_KEY_LMENU = DIK_LMENU,
			NOISE_KEY_LSHIFT = DIK_LSHIFT,
			NOISE_KEY_LWIN = DIK_LWIN,
			NOISE_KEY_M = DIK_M,
			NOISE_KEY_MAIL = DIK_MAIL,
			NOISE_KEY_MEDIASELECT = DIK_MEDIASELECT,
			NOISE_KEY_MEDIASTOP = DIK_MEDIASTOP,
			NOISE_KEY_MINUS = DIK_MINUS,
			NOISE_KEY_MULTIPLY = DIK_MULTIPLY,
			NOISE_KEY_MUTE = DIK_MUTE,
			NOISE_KEY_MYCOMPUTER = DIK_MYCOMPUTER,
			NOISE_KEY_N = DIK_N,
			NOISE_KEY_NEXT = DIK_NEXT,
			NOISE_KEY_NEXTTRACK = DIK_NEXTTRACK,
			NOISE_KEY_NOCONVERT = DIK_NOCONVERT,
			NOISE_KEY_NUMLOCK = DIK_NUMLOCK,
			NOISE_KEY_NUMPAD0 = DIK_NUMPAD0,
			NOISE_KEY_NUMPAD1 = DIK_NUMPAD1,
			NOISE_KEY_NUMPAD2 = DIK_NUMPAD2,
			NOISE_KEY_NUMPAD3 = DIK_NUMPAD3,
			NOISE_KEY_NUMPAD4 = DIK_NUMPAD4,
			NOISE_KEY_NUMPAD5 = DIK_NUMPAD5,
			NOISE_KEY_NUMPAD6 = DIK_NUMPAD6,
			NOISE_KEY_NUMPAD7 = DIK_NUMPAD7,
			NOISE_KEY_NUMPAD8 = DIK_NUMPAD8,
			NOISE_KEY_NUMPAD9 = DIK_NUMPAD9,
			NOISE_KEY_NUMPADCOMMA = DIK_NUMPADCOMMA,
			NOISE_KEY_NUMPADENTER = DIK_NUMPADENTER,
			NOISE_KEY_NUMPADEQUALS = DIK_NUMPADEQUALS,
			NOISE_KEY_O = DIK_O,
			NOISE_KEY_OEM_102 = DIK_OEM_102,
			NOISE_KEY_P = DIK_P,
			NOISE_KEY_PAUSE = DIK_PAUSE,
			NOISE_KEY_PERIOD = DIK_PERIOD,
			NOISE_KEY_PLAYPAUSE = DIK_PLAYPAUSE,
			NOISE_KEY_POWER = DIK_POWER,
			NOISE_KEY_PREVTRACK = DIK_PREVTRACK,
			NOISE_KEY_PRIOR = DIK_PRIOR,
			NOISE_KEY_Q = DIK_Q,
			NOISE_KEY_R = DIK_R,
			NOISE_KEY_RBRACKET = DIK_RBRACKET,
			NOISE_KEY_RCONTROL = DIK_RCONTROL,
			NOISE_KEY_RETURN = DIK_RETURN,
			NOISE_KEY_RIGHT = DIK_RIGHT,
			NOISE_KEY_RMENU = DIK_RMENU,
			NOISE_KEY_RSHIFT = DIK_RSHIFT,
			NOISE_KEY_RWIN = DIK_RWIN,
			NOISE_KEY_S = DIK_S,
			NOISE_KEY_SCROLL = DIK_SCROLL,
			NOISE_KEY_SEMICOLON = DIK_SEMICOLON,
			NOISE_KEY_SLASH = DIK_SLASH,
			NOISE_KEY_SLEEP = DIK_SLEEP,
			NOISE_KEY_SPACE = DIK_SPACE,
			NOISE_KEY_STOP = DIK_STOP,
			NOISE_KEY_SUBTRACT = DIK_SUBTRACT,
			NOISE_KEY_SYSRQ = DIK_SYSRQ,
			NOISE_KEY_T = DIK_T,
			NOISE_KEY_TAB = DIK_TAB,
			NOISE_KEY_U = DIK_U,
			NOISE_KEY_UNDERLINE = DIK_UNDERLINE,
			NOISE_KEY_UNLABELED = DIK_UNLABELED,
			NOISE_KEY_UP = DIK_UP,
			NOISE_KEY_V = DIK_V,
			NOISE_KEY_VOLUMEDOWN = DIK_VOLUMEDOWN,
			NOISE_KEY_VOLUMEUP = DIK_VOLUMEUP,
			NOISE_KEY_W = DIK_W,
			NOISE_KEY_WAKE = DIK_WAKE,
			NOISE_KEY_WEBBACK = DIK_WEBBACK,
			NOISE_KEY_WEBFAVORITES = DIK_WEBFAVORITES,
			NOISE_KEY_WEBFORWARD = DIK_WEBFORWARD,
			NOISE_KEY_WEBHOME = DIK_WEBHOME,
			NOISE_KEY_WEBREFRESH = DIK_WEBREFRESH,
			NOISE_KEY_WEBSEARCH = DIK_WEBSEARCH,
			NOISE_KEY_WEBSTOP = DIK_WEBSTOP,
			NOISE_KEY_X = DIK_X,
			NOISE_KEY_Y = DIK_Y,
			NOISE_KEY_YEN = DIK_YEN,
			NOISE_KEY_Z = DIK_Z,
		};

		/*_declspec(dllexport)*/extern UINT MapDInputScanCodeToAscii(UINT scanCode, bool isCapital);

	}
}