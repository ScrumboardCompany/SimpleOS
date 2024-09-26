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
}

SimpleOS::size_t Terminal::get_pos() {
	return pos;
}

void Terminal::execute_command(const char* command) {
	int size = 0;

	char** args_of_command = split(command, ' ', &size);

	if (!get_size(args_of_command)) return;

	if (strcmp(args_of_command[0], "color") == 0)
		color(args_of_command);

	//else if (strcmp(args_of_command[0], "echo") == 0)
		

	else if (strcmp(args_of_command[0], "clear") == 0) {
		clear();
	}

	else { 
		new_line();
		print(strcat(args_of_command[0], " invalid command"));
		return;
	}
	new_line();
};

void SimpleOS::Terminal::color(char** args) {
	if (!get_size(args) == 1) {
		new_line("Invalid arguments count");
		return;
	}

	char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

	Color new_color = to_color(atoi(args[1]));

	for (size_t i = 0; i < pos; ++i) {
		buffer[i * 2 + 1] = (uint8_t)new_color;
	}

	terminal_color = new_color;
}

Terminal::Color Terminal::to_color(int code) {
	switch (code) {
	case 0:  return Color::Black;
	case 1:  return Color::Blue;
	case 2:  return Color::Green;
	case 3:  return Color::Cyan;
	case 4:  return Color::Red;
	case 5:  return Color::Magenta;
	case 6:  return Color::Brown;
	case 7:  return Color::Grey;
	case 8:  return Color::DarkGrey;
	case 9:  return Color::LightBlue;
	case 10: return Color::LightGreen;
	case 11: return Color::LightCyan;
	case 12: return Color::LightRed;
	case 13: return Color::LightMagenta;
	case 14: return Color::Yellow;
	case 15: return Color::White;
	default: return Color::Grey;
	}
}

SimpleOS::size_t Terminal::pos = 0;
Terminal::Color Terminal::terminal_color = Terminal::Color::Grey;