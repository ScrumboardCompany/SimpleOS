#include "terminal/terminal.h"
#include "utils/utils.h"
#include "libs/io/io.h"
#include "devices/keyboard.h"

using namespace SimpleOS;

void Terminal::reload(char c, Color char_color, Color background_color) {
	char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

	uint8_t color = ((uint8_t)background_color << 4) | (uint8_t)char_color;

	for (size_t i = 0; i < WIDTH * HEIGHT; ++i) {
		buffer[i * 2] = c;
		buffer[i * 2 + 1] = color;
	}
}

void Terminal::clear() {
	reload(' ', terminal_color, bg_color);
	set_pos(0);
	set_buffer_pos(0);
	//clear_highlighted_buffer();
	//restore_default_bg_color();
}

void Terminal::execute_command(const string& command) {

	vector<string> command_split = split(command, ' ');
	size_t size_command_split = command_split.size();

	if (!size_command_split) return;

	vector<string> args;
	to_args(command_split, args);

	call_command(command_split[0], args);
};

void Terminal::delete_char(size_t pos) {
	if (pos > 0) {
		char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

		--pos;
		--command.buffer_pos;

		for (size_t i = pos; i < WIDTH * HEIGHT - 1; ++i) {
			buffer[i * 2] = buffer[(i + 1) * 2];     
			buffer[i * 2 + 1] = buffer[(i + 1) * 2 + 1];
		}

		buffer[(WIDTH * HEIGHT - 1) * 2] = ' ';
		buffer[(WIDTH * HEIGHT - 1) * 2 + 1] = ((uint8_t)bg_color << 4) | (uint8_t)terminal_color;

		Terminal::pos = pos;
		move_cursor(pos);
	}
}

void Terminal::textdelete_char(size_t pos) {
	if (pos > 0) {
		char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

		--pos;
		--command.buffer_pos;

		size_t current_line_end = (pos / WIDTH + 1) * WIDTH;

		for (size_t i = pos; i < current_line_end - 1; ++i) {
			buffer[i * 2] = buffer[(i + 1) * 2];
			buffer[i * 2 + 1] = buffer[(i + 1) * 2 + 1];
		}

		buffer[(current_line_end - 1) * 2] = ' '; 
		buffer[(current_line_end - 1) * 2 + 1] = ((uint8_t)bg_color << 4) | (uint8_t)terminal_color; 

		Terminal::pos = pos;
		move_cursor(pos);
	}
}

void Terminal::delete_chars(size_t pos, size_t count) {
	if (pos == 0 || count == 0) return;

	if (pos < count) {
		count = pos;
	}

	char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

	pos -= count;
	command.buffer_pos -= count;

	for (size_t i = pos; i < WIDTH * HEIGHT - count; ++i) {
		buffer[i * 2] = buffer[(i + count) * 2];
		buffer[i * 2 + 1] = buffer[(i + count) * 2 + 1];
	}

	for (size_t i = WIDTH * HEIGHT - count; i < WIDTH * HEIGHT; ++i) {
		buffer[i * 2] = ' ';
		buffer[i * 2 + 1] = ((uint8_t)bg_color << 4) | (uint8_t)terminal_color;
	}

	Terminal::pos = pos;
	move_cursor(pos);
}

void Terminal::delete_line() {
	size_t line_start = (pos / WIDTH) * WIDTH;
	char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

	for (size_t i = 0; i < WIDTH; ++i) {
		buffer[(line_start + i) * 2] = ' ';
	}

	set_pos(line_start);
	set_buffer_pos(0);
}

size_t Terminal::get_pos() {
	return pos;
}

size_t Terminal::get_buffer_pos() {
	return command.buffer_pos;
}

void Terminal::set_pos(size_t pos) {
	Terminal::pos = pos;
	move_cursor(pos);
}

void Terminal::set_buffer_pos(size_t pos) {
	command.buffer_pos = pos;
}

ssize_t Terminal::get_highlighted_buffer_pos() {
	return command.highlighted_buffer_pos;
}

size_t Terminal::get_highlighted_buffer_start_pos() {
	return command.highlighted_buffer_start_pos;
}

void Terminal::set_highlighted_buffer_pos(ssize_t pos) {
	command.highlighted_buffer_pos = pos;
}

void Terminal::clear_highlighted_buffer() {
	command.highlighted_buffer = "";
	command.highlighted_buffer_pos = 0;
	command.highlighted_buffer_start_pos = 0;
}

void Terminal::delete_highlighted_text() {

	if (Terminal::command.buffer.size() == Terminal::command.highlighted_buffer.size()) {
		Terminal::delete_line();
		Terminal::print(Terminal::get_pre_arrow_text());
		Terminal::command.buffer = "";
	}
	else if (Terminal::command.highlighted_buffer_pos < 0) {
		size_t start = Terminal::command.highlighted_buffer_start_pos + Terminal::command.highlighted_buffer_pos;
		size_t current_buffer_pos = Terminal::get_buffer_pos();
		// Can do the same thing with the pos

		for (size_t i = start; i < Terminal::command.highlighted_buffer_start_pos; i++) {
			//Terminal::delete_char(Terminal::get_pos() + 1);
			Keyboard::is_console_mode ? Terminal::delete_char(Terminal::get_pos()) : Terminal::textdelete_char(Terminal::get_pos());

			Terminal::command.buffer.pop(start - 1);
		}

		Terminal::set_buffer_pos(current_buffer_pos);
	}
	else if (Terminal::command.highlighted_buffer_pos > 0) {
		size_t start = Terminal::command.highlighted_buffer_start_pos;

		for (size_t i = start; i < Terminal::command.highlighted_buffer_pos + start; i++) {
			Keyboard::is_console_mode ? Terminal::delete_char(Terminal::get_pos()) : Terminal::textdelete_char(Terminal::get_pos());

			Terminal::command.buffer.pop(start - 1);
		}
	}

	Terminal::clear_highlighted_buffer();
	Terminal::restore_default_bg_color();
}

void Terminal::move_cursor(size_t pos) {
	uint32_t position = (uint32_t)pos;

	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t)(position & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t)((position >> 8) & 0xFF));
}

size_t Terminal::pos = 0;
size_t Terminal::current_line = 0;
Terminal::Color Terminal::terminal_color = Terminal::Color::Grey;
Terminal::Color Terminal::bg_color = Terminal::Color::Black;

string Terminal::pre_arrow_text = ">";

Terminal::Command Terminal::command;