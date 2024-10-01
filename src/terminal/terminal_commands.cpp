#include "terminal/terminal_commands.h"
#include "terminal/terminal.h"
#include "utils/utils.h"
#include "fs/fs.h"

using namespace SimpleOS;

void SimpleOS::__command_color(char** args) {

	if (__check_argc(args, 1)) {
		Terminal::Color new_color = Terminal::to_color(atoi(args[0]));
		Terminal::set_and_fill_terminal_color(new_color);
	}
}

void SimpleOS::__command_bgcolor(char** args) {

	if (__check_argc(args, 1)) {
		Terminal::Color new_color = Terminal::to_color(atoi(args[0]));
		Terminal::fill_bg(new_color);
	}
}

void SimpleOS::__command_clear(char** args) {

	if (__check_argc(args, 0))
		Terminal::clear();
}

void SimpleOS::__command_mkfile(char** args) {

	if (__check_argc(args, 1)) {

		if (FileSystem::create_file(args[0], "Hello from mkfile"))
			Terminal::lnprint("Good mkfile");

		else Terminal::print("Invalid mkfile");
	}
}

void SimpleOS::__command_rdfile(char** args) {

	if (__check_argc(args, 1)) {

		char* buffer = (char*)malloc(512);
		if (FileSystem::read_file(args[0], buffer))
			Terminal::print(buffer);

		else Terminal::print("File don`t exists");
	}
}

void SimpleOS::__command_rmfile(char** args) {

	if (__check_argc(args, 1)) {

		if (FileSystem::delete_file(args[0]))
			Terminal::print("File deleted successfully");

		else Terminal::print("File was not deleted successfully");

	}

}

bool SimpleOS::__check_argc(char** args, size_t argc) {
	size_t size = get_size(args);

	if (size == argc) return true;

	Terminal::new_line();
	Terminal::print(size > argc ? "Too many arguments" : "Too few arguments");
	
	return false;
}

void Terminal::call_command(const char* key, char** args) {

	if (strcmp(key, "color") == 0) {
		__command_color(args);
	}

	else if (strcmp(key, "bgcolor") == 0) {
		__command_bgcolor(args);
	}

	else if (strcmp(key, "clear") == 0) {
		__command_clear(args);
		return;
	}
	else if (strcmp(key, "rmfile") == 0) {
		__command_rmfile(args);
	}
	else if (strcmp(key, "rdfile") == 0) {
		__command_rdfile(args);
	}
	else if (strcmp(key, "mkfile") == 0) {
		__command_mkfile(args);
	}
	else {
		new_line();
		char* error = (char*)malloc(strlen(key) + 1);
		strcpy(error, key);
		print(strcat(error, " is invalid command"));
	}
	new_line();
}