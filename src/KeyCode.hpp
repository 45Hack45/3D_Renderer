//Using GLFW keys.
namespace Engine
{
	namespace Input
	{
		typedef const int KeyCode;

		KeyCode KEY_STATE_RELEASE = 0;

		KeyCode KEY_STATE_PRESS = 1;

		KeyCode KEY_STATE_REPEAT = 2;


		namespace KeyboardCode
		{

			KeyCode 	KEY_CODE_UNKNOWN = -1;
			KeyCode 	KEY_CODE_SPACE = 32;
			KeyCode 	KEY_CODE_APOSTROPHE = 39 /* ' */;
			KeyCode 	KEY_CODE_COMMA = 44 /* , */;
			KeyCode 	KEY_CODE_MINUS = 45 /* - */;
			KeyCode 	KEY_CODE_PERIOD = 46 /* . */;
			KeyCode 	KEY_CODE_SLASH = 47 /* / */;
			KeyCode 	KEY_CODE_0 = 48;
			KeyCode 	KEY_CODE_1 = 49;
			KeyCode 	KEY_CODE_2 = 50;
			KeyCode 	KEY_CODE_3 = 51;
			KeyCode 	KEY_CODE_4 = 52;
			KeyCode 	KEY_CODE_5 = 53;
			KeyCode 	KEY_CODE_6 = 54;
			KeyCode 	KEY_CODE_7 = 55;
			KeyCode 	KEY_CODE_8 = 56;
			KeyCode 	KEY_CODE_9 = 57;
			KeyCode 	KEY_CODE_SEMICOLON = 59 /* ; */;
			KeyCode 	KEY_CODE_EQUAL = 61 /* = */;
			KeyCode 	KEY_CODE_A = 65;
			KeyCode 	KEY_CODE_B = 66;
			KeyCode 	KEY_CODE_C = 67;
			KeyCode 	KEY_CODE_D = 68;
			KeyCode 	KEY_CODE_E = 69;
			KeyCode 	KEY_CODE_F = 70;
			KeyCode 	KEY_CODE_G = 71;
			KeyCode 	KEY_CODE_H = 72;
			KeyCode 	KEY_CODE_I = 73;
			KeyCode 	KEY_CODE_J = 74;
			KeyCode 	KEY_CODE_K = 75;
			KeyCode 	KEY_CODE_L = 76;
			KeyCode 	KEY_CODE_M = 77;
			KeyCode 	KEY_CODE_N = 78;
			KeyCode 	KEY_CODE_O = 79;
			KeyCode 	KEY_CODE_P = 80;
			KeyCode 	KEY_CODE_Q = 81;
			KeyCode 	KEY_CODE_R = 82;
			KeyCode 	KEY_CODE_S = 83;
			KeyCode 	KEY_CODE_T = 84;
			KeyCode 	KEY_CODE_U = 85;
			KeyCode 	KEY_CODE_V = 86;
			KeyCode 	KEY_CODE_W = 87;
			KeyCode 	KEY_CODE_X = 88;
			KeyCode 	KEY_CODE_Y = 89;
			KeyCode 	KEY_CODE_Z = 90;
			KeyCode 	KEY_CODE_LEFT_BRACKET = 91 /* [ */;
			KeyCode 	KEY_CODE_BACKSLASH = 92 /* \ */;
			KeyCode 	KEY_CODE_RIGHT_BRACKET = 93 /* ] */;
			KeyCode 	KEY_CODE_GRAVE_ACCENT = 96 /* ` */;
			KeyCode 	KEY_CODE_WORLD_1 = 161 /* non-US #1 */;
			KeyCode 	KEY_CODE_WORLD_2 = 162 /* non-US #2 */;
			KeyCode 	KEY_CODE_ESCAPE = 256;
			KeyCode 	KEY_CODE_ENTER = 257;
			KeyCode 	KEY_CODE_TAB = 258;
			KeyCode 	KEY_CODE_BACKSPACE = 259;
			KeyCode 	KEY_CODE_INSERT = 260;
			KeyCode 	KEY_CODE_DELETE = 261;
			KeyCode 	KEY_CODE_RIGHT = 262;
			KeyCode 	KEY_CODE_LEFT = 263;
			KeyCode 	KEY_CODE_DOWN = 264;
			KeyCode 	KEY_CODE_UP = 265;
			KeyCode 	KEY_CODE_PAGE_UP = 266;
			KeyCode 	KEY_CODE_PAGE_DOWN = 267;
			KeyCode 	KEY_CODE_HOME = 268;
			KeyCode 	KEY_CODE_END = 269;
			KeyCode 	KEY_CODE_CAPS_LOCK = 280;
			KeyCode 	KEY_CODE_SCROLL_LOCK = 281;
			KeyCode 	KEY_CODE_NUM_LOCK = 282;
			KeyCode 	KEY_CODE_PRINT_SCREEN = 283;
			KeyCode 	KEY_CODE_PAUSE = 284;
			KeyCode 	KEY_CODE_F1 = 290;
			KeyCode 	KEY_CODE_F2 = 291;
			KeyCode 	KEY_CODE_F3 = 292;
			KeyCode 	KEY_CODE_F4 = 293;
			KeyCode 	KEY_CODE_F5 = 294;
			KeyCode 	KEY_CODE_F6 = 295;
			KeyCode 	KEY_CODE_F7 = 296;
			KeyCode 	KEY_CODE_F8 = 297;
			KeyCode 	KEY_CODE_F9 = 298;
			KeyCode 	KEY_CODE_F10 = 299;
			KeyCode 	KEY_CODE_F11 = 300;
			KeyCode 	KEY_CODE_F12 = 301;
			KeyCode 	KEY_CODE_F13 = 302;
			KeyCode 	KEY_CODE_F14 = 303;
			KeyCode 	KEY_CODE_F15 = 304;
			KeyCode 	KEY_CODE_F16 = 305;
			KeyCode 	KEY_CODE_F17 = 306;
			KeyCode 	KEY_CODE_F18 = 307;
			KeyCode 	KEY_CODE_F19 = 308;
			KeyCode 	KEY_CODE_F20 = 309;
			KeyCode 	KEY_CODE_F21 = 310;
			KeyCode 	KEY_CODE_F22 = 311;
			KeyCode 	KEY_CODE_F23 = 312;
			KeyCode 	KEY_CODE_F24 = 313;
			KeyCode 	KEY_CODE_F25 = 314;
			KeyCode 	KEY_CODE_KP_0 = 320;
			KeyCode 	KEY_CODE_KP_1 = 321;
			KeyCode 	KEY_CODE_KP_2 = 322;
			KeyCode 	KEY_CODE_KP_3 = 323;
			KeyCode 	KEY_CODE_KP_4 = 324;
			KeyCode 	KEY_CODE_KP_5 = 325;
			KeyCode 	KEY_CODE_KP_6 = 326;
			KeyCode 	KEY_CODE_KP_7 = 327;
			KeyCode 	KEY_CODE_KP_8 = 328;
			KeyCode 	KEY_CODE_KP_9 = 329;
			KeyCode 	KEY_CODE_KP_DECIMAL = 330;
			KeyCode 	KEY_CODE_KP_DIVIDE = 331;
			KeyCode 	KEY_CODE_KP_MULTIPLY = 332;
			KeyCode 	KEY_CODE_KP_SUBTRACT = 333;
			KeyCode 	KEY_CODE_KP_ADD = 334;
			KeyCode 	KEY_CODE_KP_ENTER = 335;
			KeyCode 	KEY_CODE_KP_EQUAL = 336;
			KeyCode 	KEY_CODE_LEFT_SHIFT = 340;
			KeyCode 	KEY_CODE_LEFT_CONTROL = 341;
			KeyCode 	KEY_CODE_LEFT_ALT = 342;
			KeyCode 	KEY_CODE_LEFT_SUPER = 343;
			KeyCode 	KEY_CODE_RIGHT_SHIFT = 344;
			KeyCode 	KEY_CODE_RIGHT_CONTROL = 345;
			KeyCode 	KEY_CODE_RIGHT_ALT = 346;
			KeyCode 	KEY_CODE_RIGHT_SUPER = 347;
			KeyCode 	KEY_CODE_MENU = 348;
		}

		namespace MouseButtonCode{
			KeyCode 	MOUSE_CODE_BUTTON_1 = 0;
			KeyCode 	MOUSE_CODE_BUTTON_2 = 1;
			KeyCode 	MOUSE_CODE_BUTTON_3 = 2;
			KeyCode 	MOUSE_CODE_BUTTON_4 = 3;
			KeyCode 	MOUSE_CODE_BUTTON_5 = 4;
			KeyCode 	MOUSE_CODE_BUTTON_6 = 5;
			KeyCode 	MOUSE_CODE_BUTTON_7 = 6;
			KeyCode 	MOUSE_CODE_BUTTON_8 = 7;
			KeyCode 	MOUSE_CODE_BUTTON_LAST = MOUSE_CODE_BUTTON_8;
			KeyCode 	MOUSE_CODE_BUTTON_LEFT = MOUSE_CODE_BUTTON_1;
			KeyCode 	MOUSE_CODE_BUTTON_RIGHT = MOUSE_CODE_BUTTON_2;
			KeyCode 	MOUSE_CODE_BUTTON_MIDDLE = MOUSE_CODE_BUTTON_3;
		}
	}
}

