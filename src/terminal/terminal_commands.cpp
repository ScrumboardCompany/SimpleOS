#include "terminal/terminal_commands.h"
#include "terminal/terminal.h"
#include "utils/utils.h"
#include "fs/fs.h"
#include "fs/disk.h"

using namespace SimpleOS;

void Terminal_commands::__command_color(char** args) {

	if (__check_argc(args, 2)) {
		Terminal::Color new_color = Terminal::to_color(atoi(args[1]));

		if (strcmp(args[0], "text") == 0)
			Terminal::set_and_fill_terminal_color(new_color);
		else if (strcmp(args[0], "bg") == 0)
			Terminal::set_and_fill_bg_color(new_color);
	}
}

void Terminal_commands::__command_fill(char** args) {

	if (__check_argc(args, 2)) {
		Terminal::Color new_color = Terminal::to_color(atoi(args[1]));

		if (strcmp(args[0], "text") == 0)
			Terminal::fill_terminal_color(new_color);
		else if (strcmp(args[0], "bg") == 0)
			Terminal::fill_bg_color(new_color);
	}
}

void Terminal_commands::__command_set(char** args) {

	if (__check_argc(args, 2)) {
		Terminal::Color new_color = Terminal::to_color(atoi(args[1]));

		if (strcmp(args[0], "text") == 0)
			Terminal::set_terminal_color(new_color);
		else if (strcmp(args[0], "bg") == 0)
			Terminal::set_bg_color(new_color);
	}
}

void Terminal_commands::__command_clear(char** args) {

	if (__check_argc(args, 0))
		Terminal::clear();
}

void Terminal_commands::__command_reset(char** args) {

	if (__check_argc(args, 0)) {
		Terminal::reload(' ', Terminal::Color::Grey, Terminal::Color::Black);
		Terminal::pos = 0;

		Terminal::terminal_color = Terminal::Color::Grey;
		Terminal::bg_color = Terminal::Color::Black;
	}
}

void Terminal_commands::__command_echo(char** args) {

	if (__check_argc(args, 1)) {
		Terminal::lnprint(args[0]);
	}
}

void SimpleOS::Terminal_commands::__command_help(char** args) {

	if (__check_argc(args, 0)) {
		Terminal::lnprint("color [spec arg] [number] - set and fill color to the specified argument");
		Terminal::lnprint("set  [spec arg] [number] - set color to the specified argument");
		Terminal::lnprint("fill [spec arg] [number] - fill color to the specified argument");
		Terminal::lnprint("clear - clears all text in the console");
		Terminal::lnprint("reset - clears all text in the console and resets the colors");
		Terminal::lnprint("echo [text] - prints the passed argument to the console");
	}
}

void SimpleOS::Terminal_commands::__command_cat(char** args) {

	if (__check_argc(args, 1)) {
		int code = atoi(args[0]);

		switch (code) {
		case 1:
			Terminal::print(R"(
              /\/\
		 /\   / o o \
		//\\  \~(*)~/
		/  \ / ^   /
			| \|| ||
			\ '|| ||
			 \)()-()))");
			break;
		case 2:
			Terminal::print(R"(
            /\_____/\
           /  o   o  \
          ( ==  ^  == )
           )         (
          (           )
         ( (  )   (  ) )
        (__(__)___(__)__))");
			break;
		case 3:
			Terminal::print(R"(
		 /\     /\
		{  `---'  }
		{  O   O  }
		~~>  V  <~~
		 \  \|/  /
		  `-----'__
		  /     \  `^\_
		 {       }\ |\_\_   W
		 |  \_/  |/ /  \_\_( )
		  \__/  /(_E     \__/
			(  /
			 MM)");
			break;

		case 4:
			Terminal::print(R"(
			                      _
			                     | \
			                     | |
			                     | |
		  |\                    | |
		 /, ~\                 / /
		X     `-.....-------./ /
		 ~-. ~  ~              |
			\             /    |
			 \  /_     ___\   /
			 | /\ ~~~~~   \ |
			 | | \        || |
			 | |\ \       || )
			(_/ (_/      ((_/)");
			break;
		default:
			Terminal::print("Invalid cat");
			break;
		}
	}
}

void Terminal_commands::__command_mkfile(char** args) {

	if (__check_argc(args, 1)) {

		if (FileSystem::create_file(args[0], "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAABBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"))
			Terminal::lnprint("Good mkfile");
	}
}

void Terminal_commands::__command_rdfile(char** args) {

	if (__check_argc(args, 1)) {

		string buffer;
		if (FileSystem::read_file(args[0], buffer))
			Terminal::lnprint(buffer);
	}
}

void Terminal_commands::__command_rmfile(char** args) {

	if (__check_argc(args, 1)) {

		if (FileSystem::delete_file(args[0]))
			Terminal::lnprint("File deleted successfully");
	}
}

void Terminal_commands::__command_wrfile(char** args) {

	if (__check_argc(args, 2)) {

		if (FileSystem::write_to_file(args[0], args[1]))
			Terminal::lnprint("Successfully write to file");
	}
}

void Terminal_commands::__command_apfile(char** args) {

	if (__check_argc(args, 2)) {

		if (FileSystem::append_to_file(args[0], args[1]))
			Terminal::lnprint("Successfully append to file");
	}
}

void Terminal_commands::__command_exfile(char** args) {

	if (__check_argc(args, 1)) {

		if (FileSystem::exist(args[0]))
			Terminal::lnprint("File exist");
		else Terminal::lnprint("File don`t exist");
	}
}

void Terminal_commands::__command_format(char** args) {

	if (__check_argc(args, 0)) {

		if (FileSystem::format())
			Terminal::lnprint("Format successfully");
	}
}

bool Terminal_commands::__check_argc(char** args, size_t argc) {
	size_t size = get_size(args);

	if (size == argc) return true;

	Terminal::lnprint(size > argc ? "Too many arguments" : "Too few arguments");
	
	return false;
}

void Terminal::call_command(const char* key, char** args) {

	if (strcmp(key, "color") == 0) {
		Terminal_commands::__command_color(args);
	}

	else if (strcmp(key, "fill") == 0) {
		Terminal_commands::__command_fill(args);
	}

	else if (strcmp(key, "set") == 0) {
		Terminal_commands::__command_set(args);
	}

	else if (strcmp(key, "clear") == 0) {
		Terminal_commands::__command_clear(args);
		return;
	}

	else if (strcmp(key, "reset") == 0) {
		Terminal_commands::__command_reset(args);
		return;
	}

	else if (strcmp(key, "echo") == 0) {
		Terminal_commands::__command_echo(args);
	}

	else if (strcmp(key, "help") == 0) {
		Terminal_commands::__command_help(args);
	}

	else if (strcmp(key, "cat") == 0) {
		Terminal_commands::__command_cat(args);
	}
	
	else if (strcmp(key, "mkfile") == 0) {
		Terminal_commands::__command_mkfile(args);
	}

	else if (strcmp(key, "rdfile") == 0) {
		Terminal_commands::__command_rdfile(args);
	}

	else if (strcmp(key, "rmfile") == 0) {
		Terminal_commands::__command_rmfile(args);
	}

	else if (strcmp(key, "wrfile") == 0) {
		Terminal_commands::__command_wrfile(args);
	}

	else if (strcmp(key, "apfile") == 0) {
		Terminal_commands::__command_wrfile(args);
	}

	else if (strcmp(key, "exfile") == 0) {
		Terminal_commands::__command_wrfile(args);
	}

	else if (strcmp(key, "format") == 0) {
		Terminal_commands::__command_format(args);
	}

	else if (strcmp(key, "rdsector") == 0) {
		char buffer[512];

		ata_read_sector(0, buffer);

		Terminal::lnprint(buffer);
	}

	else if (strcmp(key, "cat") == 0) {
		lnprint(",_     _\n| \\_, -~/ \n/ _  _ | , --.\n(@  @) / , -'\n\  _T_ / -._((\n/ `. \\\n| _  \ | \n\ \, /      |\n|| |-_\__ / \n((_ / `(____, -'\n");
	}

	else {
		char* error = (char*)malloc(strlen(key) + 1);
		strcpy(error, key);
		lnprint(strcat(error, " is invalid command\nenter help for print all commands"));
	}
	new_line();
}