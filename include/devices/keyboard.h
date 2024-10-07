#pragma once

#ifndef _KEYBOARD_
#define _KEYBOARD_

#define _NO_CHAR '\0'

#include "utils/utils.h"
#include "utils/typedef.h"
#include "libs/vector/vector.h"
#include "libs/string/class.h"

#include <stdint.h>

namespace SimpleOS {

    class Keyboard {

    public:
        enum class PressedKey : uint32_t {
			A = 0x1E,
			B = 0x30,
			C = 0x2E,
			D = 0x20,
			E = 0x12,
			F = 0x21,
			G = 0x22,
			H = 0x23,
			I = 0x17,
			J = 0x24,
			K = 0x25,
			L = 0x26,
			M = 0x32,
			N = 0x31,
			O = 0x18,
			P = 0x19,
			Q = 0x10,
			R = 0x13,
			S = 0x1F,
			T = 0x14,
			U = 0x16,
			V = 0x2F,
			W = 0x11,
			X = 0x2D,
			Y = 0x15,
			Z = 0x2C,
			// Number keys
			Num0 = 0x0B,
			Num1 = 0x02,
			Num2 = 0x03,
			Num3 = 0x04,
			Num4 = 0x05,
			Num5 = 0x06,
			Num6 = 0x07,
			Num7 = 0x08,
			Num8 = 0x09,
			Num9 = 0x0A,
			// Special keys
			Escape = 0x01,
			Space = 0x39,
			Enter = 0x1C,
			Backspace = 0x0E,
			Tab = 0x0F,
			Shift = 0x2A,
			RightShift = 0x36,
			ShiftReleased = 0xAA,
			RightShiftReleased = 0xB6,
			Ctrl = 0x1D,
			CtrlReleased = 0x9D,
			Alt = 0x38,
			AltReleased = 0xB8,
			CapsLock = 0x3A,
			F1 = 0x3B,
			F2 = 0x3C,
			F3 = 0x3D,
			F4 = 0x3E,
			F5 = 0x3F,
			F6 = 0x40,
			F7 = 0x41,
			F8 = 0x42,
			F9 = 0x43,
			F10 = 0x44,
			F11 = 0x57,
			F12 = 0x58,
			Equals = 0x0D,    
			Minus = 0x0C,     
			Slash = 0x35,     
			Backslash = 0x2B,
			Semicolon = 0x27,  
			Apostrophe = 0x28, 
			Comma = 0x33,      
			Period = 0x34,  
			LeftBracket = 0x1A,
			RightBracket = 0x1B,
			Tilde = 0x29,      
			Plus = 0x0D,     
			QuestionMark = 0x35,
			Colon = 0x27,    
			Quote = 0x28,
			// Arrow keys
			ArrowUp = 0x48,
			ArrowDown = 0x50,
			ArrowLeft = 0x4B,
			ArrowRight = 0x4D,
			// Numpad keys
			Numpad0 = 0x52,
			Numpad1 = 0x4F,
			Numpad2 = 0x50,
			Numpad3 = 0x51,
			Numpad4 = 0x4B,
			Numpad5 = 0x4C,
			Numpad6 = 0x4D,
			Numpad7 = 0x47,
			Numpad8 = 0x48,
			Numpad9 = 0x49,
			NumpadEnter = 0x9C,
			NumpadPlus = 0x4E,
			NumpadMinus = 0x4A,
			NumpadMultiply = 0x37,
			NumpadDivide = 0xB5,
			NumpadPeriod = 0x53,
			// Additional keys
			Home = 0x47,
			End = 0x4F,
			PageUp = 0x49,
			PageDown = 0x51,
			Insert = 0x52,
			Delete = 0x53,
			PrintScreen = 0xB7,
			ScrollLock = 0x46,
			Pause = 0x45,
			NumLock = 0x45
		};

        static void init_keyboard();

        static PressedKey get_key();

		static char get_key_char(PressedKey key);

		static void reset_selected_command_pos();

		static void change_mode(bool is_console_mode);

    //private:
		
		static string buffer;

		static bool is_caps_lock;
		static bool is_console_mode;

		static bool ctrl_pressed;
		static bool shift_pressed;
		static bool alt_pressed;

		static vector<string> commands;

		static size_t selected_command_pos;

	//private:
		static void __handle_arrow(bool isUp);


		static void __backspace(PressedKey key);
		static void __enter(PressedKey key);
		static void __capslock(PressedKey key);
		static void __arrow_up(PressedKey key);
		static void __arrow_down(PressedKey key);
		static void __arrow_left(PressedKey key);
		static void __arrow_right(PressedKey key);

		static void __textbackspace(PressedKey key);
		static void __textenter(PressedKey key);
		static void __textctrl(PressedKey key);

    };

	extern "C" void keyboard_handler();
}

#endif // _KEYBOARD_