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
			Ctrl = 0x1D,
			Alt = 0x38,
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
			// Arrow keys
			ArrowUp = 0x48,
			ArrowDown = 0x50,
			ArrowLeft = 0x4B,
			ArrowRight = 0x4D
		};

        static void init_keyboard();

        static PressedKey get_key();

		static char get_key_char(PressedKey key);

		static void reset_selected_command_pos();

    //private:
		
		static string buffer;

		static bool is_caps_lock;

		static vector<string> commands;

		static int selected_command_pos;

	//private:
		static void __handle_arrow(bool isUp);


		static void __backspace(PressedKey key);
		static void __enter(PressedKey key);
		static void __capslock(PressedKey key);
		static void __arrow_up(PressedKey key);
		static void __arrow_down(PressedKey key);

    };

	extern "C" void keyboard_handler();
}

#endif // _KEYBOARD_