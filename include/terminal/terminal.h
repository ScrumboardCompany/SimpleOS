#pragma once

#ifndef _TERMINAL_
#define _TERMINAL_

#include "utils/typedef.h"
#include "terminal/terminal_commands.h"
#include "libs/string/class.h"
#include "libs/vector/vector.h"
#include <stdint.h>

#define VIDEO_MEMORY_ADDRESS 0xB8000

namespace SimpleOS {

	constexpr size_t WIDTH = 80;
	constexpr size_t HEIGHT = 25;

	class Terminal {
		friend class Terminal_commands;
		friend class Keyboard;
	public:
		enum class Color : uint8_t;

	private:

		struct Command {
			string buffer;
			size_t buffer_pos;
			vector<string> commands;
			ssize_t selected_command_pos;
			string highlighted_buffer;
			ssize_t highlighted_buffer_pos;
			size_t highlighted_buffer_start_pos;
			string clipboard;
		};

		static size_t pos;
		static size_t current_line;
		static Color terminal_color;
		static Color bg_color;

		static vector<string> lines_buffer;
		static size_t scroll_position;

		static Command command;
		
		static void to_args(vector<string>& command_split, vector<string>& args);

	public:
		static string pre_arrow_text;

		static Color to_color(int code);

		static Color to_color(const string& code);

		static void print(const char* msg);

		static void print(char c, size_t pos);

		static void print(const string& str, size_t pos);

		static void print(char c);

		static void input_print(char c);

		static void text_print(char c);

		static void print(int n);

		static void print(const string& string);

		static void println(const char* msg);

		static void println(char c);

		static void println(int n);

		static void println(const string& string);

		static void lnprint(const char* msg);

		static void lnprint(char c);

		static void lnprint(int n);

		static void lnprint(const string& string);

		static void lnprintln(const char* msg);

		static void lnprintln(char c);

		static void lnprintln(int n);

		static void lnprintln(const string& string);



		static void reload(char c, Color char_color, Color background_color);

		static void clear();

		static void delete_char(size_t pos);

		static void textdelete_char(size_t pos);

		static void delete_chars(size_t pos, size_t count);

		static void delete_line();

		static void new_line();

		static void execute_command(const string& command);

		static size_t get_pos();

		static size_t get_buffer_pos();

		static void set_pos(size_t pos);

		static void set_buffer_pos(size_t pos);

		static ssize_t get_highlighted_buffer_pos();

		static size_t get_highlighted_buffer_start_pos();

		static void set_highlighted_buffer_pos(ssize_t pos);

		static void clear_highlighted_buffer();

		static void delete_highlighted_text();

		static void move_cursor(size_t position);

		static void update_screen();

		static void set_and_fill_terminal_color(Color new_color);

		static void fill_terminal_color(Color new_color);

		static void set_terminal_color(Color new_color);

		static void set_and_fill_bg_color(Color new_color);

		static void fill_bg_color(Color new_color);

		static void set_bg_color(Color new_color);

		static void fill_bg_color_at(size_t pos, Color new_color);

		static void restore_default_bg_color();

		static void restore_bg_color_at(size_t pos);

		static const string& get_pre_arrow_text();

		static void set_pre_arrow_text(const string& new_text);

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
			White = 0x0F,
			None = 63
		};

	private: // commands

		static void call_command(const string& key, vector<string>& argv);
	};
}

#endif // _TERMINAL_