#include "terminal/terminal_commands.h"
#include "terminal/terminal.h"
#include "utils/utils.h"

using namespace SimpleOS;

void SimpleOS::__command_color(char** args) {

	if (__check_argc(args, 1)) {
		Terminal::Color new_color = Terminal::to_color(atoi(args[0]));
		Terminal::set_and_fill_terminal_color(new_color);
	}
}

void SimpleOS::__command_clear(char** args) {

	if (__check_argc(args, 0)) {
		Terminal::clear();
	}
}

bool SimpleOS::__check_argc(char** args, size_t argc) {
	size_t size = get_size(args);

	if (size == argc) return true;

	Terminal::print(size > argc ? "Too many arguments" : "Too few arguments");
	
	return false;
}

void Terminal::call_command(const char* key, char** args) {

	if (strcmp(key, "color") == 0)
		__command_color(args);
	else if (strcmp(key, "clear") == 0)
		__command_clear(args);
}