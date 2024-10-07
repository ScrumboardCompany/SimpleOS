#include "terminal/terminal.h"
#include "libs/string/string.h"
#include "libs/string/class.h"

using namespace SimpleOS;

void Terminal::print(const char* msg) {
	while (*msg) {
		print(*msg);
		++msg;
	}
}

void Terminal::print(char c, size_t pos) {
	char* buffer = (char*)VIDEO_MEMORY_ADDRESS;
	uint8_t color = ((uint8_t)bg_color << 4) | (uint8_t)terminal_color;

	buffer[pos * 2] = c;
	buffer[pos * 2 + 1] = color;
}

void Terminal::print(char c) {
	if (c == '\n') {
		new_line();
	} else if (c == '\t') {
		print("   ");
	}
	else {
		if (pos < WIDTH* HEIGHT - 1) {
			for (size_t i = WIDTH * HEIGHT - 1; i > pos; --i) {
				char* buffer = (char*)VIDEO_MEMORY_ADDRESS;
				buffer[i * 2] = buffer[(i - 1) * 2];
				buffer[i * 2 + 1] = buffer[(i - 1) * 2 + 1];
			}
		}

		print(c, pos);

		pos++;
		command.buffer_pos++;
	}

	move_cursor(pos);
}

void Terminal::print(int n) {
	char msg[10];
	itoa(n, msg, 10);
	print(msg);
}

void Terminal::print(const string& str) {
	print(str.c_str());
}

void Terminal::println(const char* msg) {
	print(msg);

	new_line();
}

void Terminal::println(char c) {
	print(c);

	new_line();
};

void Terminal::println(int n) {
	print(n);

	new_line();
}

void Terminal::println(const string& string) {
	print(string);

	new_line();
}

void Terminal::lnprint(const char* msg) {
	new_line();

	print(msg);
}

void Terminal::lnprint(char c){
	new_line();

	print(c);
}

void Terminal::lnprint(int n) {
	new_line();

	print(n);
}

void Terminal::lnprint(const string& string) {
	new_line();

	print(string);
}

void Terminal::lnprintln(const char* msg) {
	new_line();

	print(msg);

	new_line();
}

void Terminal::lnprintln(char c) {
	new_line();

	print(c);

	new_line();
};

void Terminal::lnprintln(int n) {
	new_line();

	print(n);

	new_line();
}

void Terminal::lnprintln(const string& string) {
	new_line();

	print(string);

	new_line();
};