#include "terminal/terminal.h"
#include "utils/utils.h"

using namespace SimpleOS;

void Terminal::reload(char c, Color color) {
	char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

	for (size_t i = 0; i < WIDTH * HEIGHT; ++i) {
		buffer[i * 2] = c;
		buffer[i * 2 + 1] = (uint8_t)color;
	}
}

void Terminal::clear() {
	reload(' ', terminal_color);
	pos = 0;
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

void Terminal::delete_char(size_t pos) {
	if (pos > 0) {
		char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

		--pos;
		buffer[pos * 2] = ' ';

		Terminal::pos = pos;
	}
}

void Terminal::delete_line() {
	const size_t WIDTH = 80;
	size_t line_start = (pos / WIDTH) * WIDTH;
	char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

	for (size_t i = 0; i < WIDTH; ++i) {
		buffer[(line_start + i) * 2] = ' ';
		buffer[(line_start + i) * 2 + 1] = (uint8_t)terminal_color;
	}

	pos = line_start;
}

size_t Terminal::get_pos() {
	return pos;
}

size_t Terminal::pos = 0;
Terminal::Color Terminal::terminal_color = Terminal::Color::Grey;