#include "terminal/terminal.h"
#include "libs/memory/memory.h"
#include "libs/string/string.h"

using namespace SimpleOS;

void Terminal::to_args(char** command_split, char** args, size_t size_command_split) {
	for (size_t i = 1; i < size_command_split; i++) {

		args[i - 1] = (char*)malloc((strlen(command_split[i]) + 1) * sizeof(char));

		strcpy(args[i - 1], command_split[i]);
	}
	args[size_command_split - 1] = NULL;
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

		set_pos((HEIGHT - 1) * WIDTH);
		set_buffer_pos(0);
	}
	else {
		set_pos((pos / WIDTH + 1) * WIDTH);
		set_buffer_pos(0);
	}
}