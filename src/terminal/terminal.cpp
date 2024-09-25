#include "terminal/terminal.h"
#include "utils/utils.h"

using namespace SimpleOS;

void Terminal::print(const char* msg) {
	while (*msg) {
		print(*msg);
		++msg;
	}
}

void Terminal::print(char c, size_t pos) {
	char* buffer = (char*)VIDEO_MEMORY_ADDRESS;
	buffer[pos * 2] = c;
	buffer[pos * 2 + 1] = (uint8_t)Color::Grey;
}

void Terminal::print(char c) {
	print(c, pos++);
}

void Terminal::print(int n) {
	char msg[10];
	print(itoa(n, msg, 10));
}

void Terminal::reload(char c, Color color) {
	char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

	for (size_t i = 0; i < pos; ++i) {
		buffer[i * 2] = c;
		buffer[i * 2 + 1] = (uint8_t)color;
	}
}

void Terminal::clear() {
	reload(' ');
	pos = 0;
}

void Terminal::delete_char(size_t pos) {
    if (pos > 0) {
        char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

        --pos;
        buffer[pos * 2] = ' ';

        Terminal::pos = pos;
    }
}

void Terminal::new_line() {
	const size_t WIDTH = 80;
	const size_t HEIGHT = 25;

	if (pos / WIDTH >= HEIGHT) {
		char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

		for (size_t i = 1; i < HEIGHT; i++) {
			for (size_t j = 0; j < WIDTH; ++j) {
				buffer[((i - 1) * WIDTH + j) * 2] = buffer[(i * WIDTH + j) * 2];
			}
		}

		for (size_t j = 0; j < WIDTH; ++j) {
			buffer[(HEIGHT - 1) * WIDTH * 2 + j * 2] = ' ';
		}

		pos = (HEIGHT - 1) * WIDTH;
	}
	else {
		pos = (pos / WIDTH + 1) * WIDTH;
	}
	print(">");
}

SimpleOS::size_t Terminal::get_pos() {
	return pos;
}

SimpleOS::size_t Terminal::pos = 0;