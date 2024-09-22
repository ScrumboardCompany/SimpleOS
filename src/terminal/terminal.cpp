#include "terminal/terminal.h"

using namespace SimpleOS;

void Terminal::print(cstr msg) {
	while (*msg) {
		print(*msg);
		++msg;
	}
}

void Terminal::print(char c, size_t pos) {
	char* buffer = (char*)VIDEO_MEMORY_ADDRESS;
	buffer[pos * 2] = c;
	buffer[pos * 2 + 1] = 0x07;
}

void Terminal::print(char c) {
	print(c, pos++);
}

size_t Terminal::pos = 0;