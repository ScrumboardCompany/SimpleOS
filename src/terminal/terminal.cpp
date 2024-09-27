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

void Terminal::new_line() {

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

	char** command_split = split(command, ' ', &size);
	SimpleOS::size_t size_command_split = get_size(command_split);

	if (!size_command_split) return;

	char** args = (char**)malloc((size_command_split - 1) * sizeof(char*));
	to_args(command_split, args, size_command_split);

	if (strcmp(command_split[0], "color") == 0)
		color(args);

	//else if (strcmp(args_of_command[0], "echo") == 0)
		

	else if (strcmp(command_split[0], "clear") == 0) {
		clear();
	}

	else { 
		new_line();
		print(strcat(command_split[0], " invalid command"));
		return;
	}
};

void SimpleOS::Terminal::color(char** args) {
	if (get_size(args) != 1) {
		new_line();
		print("Invalid arguments count");
		return;
	}

	char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

	Color new_color = to_color(atoi(args[0]));

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

void Terminal::to_args(char** command_split, char** args, size_t size_command_split) {
	for (SimpleOS::size_t i = 1; i < size_command_split; i++) {

		args[i - 1] = (char*)malloc((strlen(command_split[i]) + 1) * sizeof(char));

		strcpy(args[i - 1], command_split[i]);
	}
	args[size_command_split - 1] = NULL;
}

SimpleOS::size_t Terminal::pos = 0;
Terminal::Color Terminal::terminal_color = Terminal::Color::Grey;