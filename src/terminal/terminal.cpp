#include "terminal/terminal.h"

using namespace SimpleOS;

void Terminal::print_string(cstr msg) {
	while (*msg) {
		print_char(*msg);
		++msg;
	}
}

void Terminal::print_char(char c, size_t pos) {
	char* buffer = (char*)VIDEO_MEMORY_ADDRESS;
	buffer[pos * 2] = c;
	buffer[pos * 2 + 1] = 0x07;
}

void Terminal::print_char(char c) {
	print_char(c, pos++);
}

size_t Terminal::pos = 0;