#include "terminal/terminal.h"
#include "libs/string/string.h"

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
	buffer[pos * 2 + 1] = (uint8_t)terminal_color;
}

void Terminal::print(char c) {
	print(c, pos++);
}

void Terminal::print(int n) {
	char msg[10];
	itoa(n, msg, 10);
	print(msg);
}