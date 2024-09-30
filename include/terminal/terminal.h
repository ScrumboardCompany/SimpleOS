#pragma once

#ifndef _TERMINAL_
#define _TERMINAL_

#include "utils/typedef.h"
#include "terminal/terminal_commands.h"
#include "libs/string/class.h"
#include <stdint.h>

#define VIDEO_MEMORY_ADDRESS 0xB8000

namespace SimpleOS {

	constexpr size_t WIDTH = 80;
	constexpr size_t HEIGHT = 25;

	class Terminal {

	public:
		enum class Color : uint8_t;

	private:

		static size_t pos;
		static Color terminal_color;
		
		static void to_args(char** command_split, char** args, size_t size_command_split);

	public:

		static Color to_color(int code);

		static void print(const char* msg);

		static void print(char c, size_t pos);

		static void print(char c);

		static void print(int n);

		static void print(const string& string);

		static void reload(char c, Color color);

		static void clear();

		static void delete_char(size_t pos);

		static void delete_line();

		static void new_line();

		static void execute_command(const char* command);

		static size_t get_pos();

		static void set_and_fill_terminal_color(Color new_color);

		static void fill_terminal_color(Color new_color);

		static void set_terminal_color(Color new_color);

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

	private: // commands

		static void call_command(const char* key, char** argv);
	};
}

#endif // _TERMINAL_