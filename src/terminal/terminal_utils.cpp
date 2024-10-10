#include "terminal/terminal.h"
#include "libs/memory/memory.h"
#include "libs/string/string.h"

using namespace SimpleOS;

void Terminal::to_args(vector<string>& command_split, vector<string>& args) {
	args = command_split;
	args.pop(0);
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
			buffer[(HEIGHT - 1) * WIDTH * 2 + j * 2] = 0; 
		}

		set_pos((HEIGHT - 1) * WIDTH);
	}
	else {
		set_pos((pos / WIDTH + 1) * WIDTH);
	}

	set_buffer_pos(0);
	current_line++;
}