#include "terminal/terminal.h"

using namespace SimpleOS;

void Terminal::print_string(cstr msg) {
	while (msg[pos])
		print_char(msg[pos++]);
}

void Terminal::print_char(char c, size_t pos) {
	buffer[pos * 2] = c;
	buffer[pos * 2 + 1] = 0x07;
}

void Terminal::print_char(char c) {
	print_char(c, pos++);
}