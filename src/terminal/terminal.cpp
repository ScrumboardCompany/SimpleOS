#include "terminal/terminal.h"
#include "utils/utils.h"
#include "IRQ/IRQ.h"

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
}

void Terminal::execute_command(const char* command) {
	int size = 0;

	char** command_split = split(command, ' ', &size);
	size_t size_command_split = get_size(command_split);

	if (!size_command_split) return;

	char** args = (char**)malloc((size_command_split - 1) * sizeof(char*));
	to_args(command_split, args, size_command_split);

	call_command(command_split[0], args);
};

void Terminal::delete_char() {
	if (pos > 0) {
		char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

		set_pos(pos - 1);
		buffer[pos * 2] = ' ';
		move_cursor(pos - 1);
	}
}

void Terminal::delete_line() {
	size_t line_start = (pos / WIDTH) * WIDTH;
	char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

	for (size_t i = 0; i < WIDTH; ++i) {
		buffer[(line_start + i) * 2] = ' ';
	}

	set_pos(line_start);
}

size_t Terminal::get_pos() {
	return pos;
}

void Terminal::set_pos(size_t pos) {
	Terminal::pos = pos;
	move_cursor(pos);
}

void Terminal::move_cursor(size_t pos) {
	uint8_t position = (uint8_t)pos + 1;

	IRQ::port_byte_out(0x3D4, 0x0F);
	IRQ::port_byte_out(0x3D5, (uint8_t)(position & 0xFF));
	IRQ::port_byte_out(0x3D4, 0x0E);
	IRQ::port_byte_out(0x3D5, (uint8_t)((position >> 8) & 0xFF));
}

size_t Terminal::pos = 0;
Terminal::Color Terminal::terminal_color = Terminal::Color::Grey;
Terminal::Color Terminal::bg_color = Terminal::Color::Black;