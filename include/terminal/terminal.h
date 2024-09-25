#pragma once

#ifndef _TERMINAL_

#include "utils/typedef.h"
#include <stdint.h>
#include <stddef.h>

#define VIDEO_MEMORY_ADDRESS 0xB8000

namespace SimpleOS {

	class Terminal {

		static size_t pos;

	public:
		enum class Color : uint8_t;

		static void print(const char* msg);

		static void print(char c, size_t pos);

		static void print(char c);

		static void print(int n);

		static void reload(char c, Color color = Color::Grey);

		static void clear();

		static void delete_char(size_t pos);

		static void new_line();

		static void execute_command(const char* command);

		static size_t get_pos();

		enum class Color : uint8_t {
			Black = 0x00,
			Blue = 0x01,
			Green = 0x02,
			Cyan = 0x03,
			Red = 0x04,
			Magenta = 0x05,
			Brown = 0x06,
			Grey = 0x07,
			DarkGrey = 0x08,
			LightBlue = 0x09,
			LightGreen = 0x0A,
			LightCyan = 0x0B,
			LightRed = 0x0C,
			LightMagenta = 0x0D,
			Yellow = 0x0E,
			White = 0x0F
		};
		private:
			static void color(char** args_of_command);

			static Color to_color(int code);

			static Color terminal_color;
	};
}

#endif // _TERMINAL_