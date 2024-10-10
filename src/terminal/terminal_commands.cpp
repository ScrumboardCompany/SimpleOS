#include "terminal/terminal_commands.h"
#include "terminal/terminal.h"
#include "utils/utils.h"
#include "fs/fs.h"
#include "fs/disk.h"
#include "devices/keyboard.h"

using namespace SimpleOS;

void Terminal_commands::__command_color(vector<string>& args) {

	if (__check_argc(args, 2)) {
		Terminal::Color new_color = Terminal::to_color(atoi(args[1]));

		if (strcmp(args[0], "text") == 0)
			Terminal::set_and_fill_terminal_color(new_color);
		else if (strcmp(args[0], "bg") == 0)
			Terminal::set_and_fill_bg_color(new_color);
	}
}

void Terminal_commands::__command_fill(vector<string>& args) {

	if (__check_argc(args, 2)) {
		Terminal::Color new_color = Terminal::to_color(atoi(args[1]));

		if (strcmp(args[0], "text") == 0)
			Terminal::fill_terminal_color(new_color);
		else if (strcmp(args[0], "bg") == 0)
			Terminal::fill_bg_color(new_color);
	}
}

void Terminal_commands::__command_set(vector<string>& args) {

	if (__check_argc(args, 2)) {
		Terminal::Color new_color = Terminal::to_color(atoi(args[1]));

		if (strcmp(args[0], "text") == 0)
			Terminal::set_terminal_color(new_color);
		else if (strcmp(args[0], "bg") == 0)
			Terminal::set_bg_color(new_color);
	}
}

void Terminal_commands::__command_clear(vector<string>& args) {

	if (__check_argc(args, 1, false)) {
		if (args[0] == "buffer") {
			Terminal::command.commands.clear();
			Keyboard::reset_selected_command_pos();
			Terminal::new_line();
		}
		else if (args[0] == "terminal") {
			Terminal::clear();
		}
		else {
			Terminal::lnprint("Invalid argument for clear");
		}
	}
	else if(__check_argc(args, 0))
		Terminal::clear();
}

void Terminal_commands::__command_reset(vector<string>& args) {

	if (__check_argc(args, 0)) {
		Terminal::reload(' ', Terminal::Color::Grey, Terminal::Color::Black);
		Terminal::set_pos(0);
		Terminal::set_buffer_pos(0);
		Terminal::clear_highlighted_buffer();
		Terminal::restore_default_bg_color();

		Terminal::terminal_color = Terminal::Color::Grey;
		Terminal::bg_color = Terminal::Color::Black;
	}
}

void Terminal_commands::__command_echo(vector<string>& args) {

	if (__check_argc(args, 1)) {
		Terminal::lnprint(args[0]);
	}
}

void Terminal_commands::__command_help(vector<string>& args) {

	if (__check_argc(args, 0)) {
		Terminal::lnprint("color [spec arg] [number] - set and fill color to the specified argument");
		Terminal::lnprint("set  [spec arg] [number] - set color to the specified argument");
		Terminal::lnprint("fill [spec arg] [number] - fill color to the specified argument");
		Terminal::lnprint("clear - clears all text in the console");
		Terminal::lnprint("reset - clears all text in the console and resets the colors");
		Terminal::lnprint("echo [text] - prints the passed argument to the console");
		Terminal::lnprint("cat [number] - prints a cat under the specified number");
		Terminal::lnprint("mkfile [name] [data] - creates a file called \"name\" and adds \"data\" there");
		Terminal::lnprint("rdfile [name] - reads data from a file named \"name\"");
		Terminal::lnprint("rmfile [name] - deletes a file named \"name\"");
		Terminal::lnprint("wrfile [name] [data] - overwrites data from a file named \"name\" with new \"data\"");
		Terminal::lnprint("apfile [name] [data] - assigns new \"data\" to data from a file named \"name\"");
		Terminal::lnprint("format - deletes all files");
		Terminal::lnprint("open [name] - opens file with name \"name\"");
	}
}

void Terminal_commands::__command_cat(vector<string>& args) {

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
			Terminal::lnprint("Invalid cat");
			break;
		}
	}
}

void Terminal_commands::__command_mkfile(vector<string>& args) {

	if (__check_argc(args, 2)) {

		if (FileSystem::create_file(args[0], args[1]))
			Terminal::lnprint("Good mkfile");
	}
}

void Terminal_commands::__command_rdfile(vector<string>& args) {

	if (__check_argc(args, 1)) {

		string buffer;
		if (FileSystem::read_file(args[0], buffer))
			Terminal::lnprint(buffer);
	}
}

void Terminal_commands::__command_rmfile(vector<string>& args) {

	if (__check_argc(args, 1)) {

		if (FileSystem::delete_file(args[0]))
			Terminal::lnprint("File deleted successfully");
	}
}

void Terminal_commands::__command_wrfile(vector<string>& args) {

	if (__check_argc(args, 2)) {

		if (FileSystem::write_to_file(args[0], args[1]))
			Terminal::lnprint("Successfully write to file");
	}
}

void Terminal_commands::__command_apfile(vector<string>& args) {

	if (__check_argc(args, 2)) {

		if (FileSystem::append_to_file(args[0], args[1]))
			Terminal::lnprint("Successfully append to file");
	}
}

void Terminal_commands::__command_exfile(vector<string>& args) {

	if (__check_argc(args, 1)) {

		if (FileSystem::file_exist(args[0]))
			Terminal::lnprint("File exist");
		else Terminal::lnprint("File don`t exist");
	}
}

void Terminal_commands::__command_format(vector<string>& args) {

	if (__check_argc(args, 0)) {

		if (FileSystem::format())
			Terminal::lnprint("Format successfully");
	}
}

void Terminal_commands::__command_mkdir(vector<string>& args) {

	if (__check_argc(args, 1)) {
		if (FileSystem::create_dir(args[0])) {
			Terminal::lnprint("Good mkdir");
		}
	}
}

void Terminal_commands::__command_rmdir(vector<string>& args) {

	if (__check_argc(args, 1)) {
		if (FileSystem::delete_dir(args[0])) {
			Terminal::lnprint("Good rmdir");
		}
	}
}

void Terminal_commands::__command_cddir(vector<string>& args) {

	if (__check_argc(args, 1)) {
		if (FileSystem::cd(args[0])) {
			Terminal::lnprint("Good cddir");
		}
	}
}

void Terminal_commands::__command_tree(vector<string>& args) {

	if (__check_argc(args, 0)) {
		
		FileSystem::tree(FileSystem::get_root());
	}
}

void Terminal_commands::__command_path(vector<string>& args) {

	if (__check_argc(args, 0)) {

		Terminal::lnprint(FileSystem::get_current_path());
	}
}

void Terminal_commands::__command_dir(vector<string>& args) {

	if (__check_argc(args, 0)) {

		FileSystem::tree(*FileSystem::get_current_directory());
	}
}

void Terminal_commands::__command_open(vector<string>& args) {

	if (__check_argc(args, 1)) {

		if (FileSystem::__check_exist(args[0])) {
			Terminal::clear();
			//Terminal::lnprint("Good clear");

			FileSystem::open_file(args[0]);
			//Terminal::lnprint("Good openfile: ");
			//Terminal::print(FileSystem::get_opened_file());

			string buffer;
			FileSystem::read_file(args[0], buffer);
			//Terminal::lnprint("Buffer: ");
			//Terminal::print(buffer);

			Keyboard::change_mode(false);

			Terminal::print(buffer);
			Terminal::command.buffer = buffer;
			//Terminal::lnprint("Keyboard buffer: ");
			//Terminal::print(Keyboard::buffer);
		}
	}
}

void Terminal_commands::__command_time(vector<string>& args) {

	if (__check_argc(args, 0)) {

		time current_time = get_time();

		Terminal::lnprint((int)current_time.hour);
		Terminal::print(':');
		Terminal::print((int)current_time.minute);
		Terminal::print(':');
		Terminal::print((int)current_time.second);

		Terminal::lnprint((int)current_time.day);
		Terminal::print('.');
		Terminal::print((int)current_time.month);
		Terminal::print('.');
		Terminal::print((int)current_time.year);
	}
}

bool Terminal_commands::__check_argc(vector<string>& args, size_t argc, bool print_error) {

	if (args.size() == argc) return true;

	if (print_error) Terminal::lnprint(args.size() > argc ? "Too many arguments" : "Too few arguments");

	return false;
}

void Terminal::call_command(const string& key, vector<string>& args) {

	if (key == "color") {
		Terminal_commands::__command_color(args);
	}

	else if (key == "set") {
		Terminal_commands::__command_set(args);
	}

	else if (key == "fill") {
		Terminal_commands::__command_fill(args);
	}

	else if (key == "clear") {
		Terminal_commands::__command_clear(args);
		return;
	}

	else if (key == "reset") {
		Terminal_commands::__command_reset(args);
		return;
	}

	else if (key == "echo") {
		Terminal_commands::__command_echo(args);
	}

	else if (key == "help") {
		Terminal_commands::__command_help(args);
	}

	else if (key == "cat") {
		Terminal_commands::__command_cat(args);
	}

	else if (key == "mkfile") {
		Terminal_commands::__command_mkfile(args);
	}

	else if (key == "rdfile") {
		Terminal_commands::__command_rdfile(args);
	}

	else if (key == "rmfile") {
		Terminal_commands::__command_rmfile(args);
	}

	else if (key == "wrfile") {
		Terminal_commands::__command_wrfile(args);
	}

	else if (key == "apfile") {
		Terminal_commands::__command_apfile(args);
	}

	else if (key == "exfile") {
		Terminal_commands::__command_exfile(args);
	}

	else if (key == "format") {
		Terminal_commands::__command_format(args);
	}

	else if (key == "open") {
		Terminal_commands::__command_open(args);
		return;
	}

	else if (key == "time") {
		Terminal_commands::__command_time(args);
	}

	else if (key == "high") {
		Terminal::lnprint((int)Terminal::command.highlighted_buffer.size());
		Terminal::lnprint(Terminal::command.highlighted_buffer);
		Terminal::lnprint((int)Terminal::command.highlighted_buffer_pos);
		Terminal::lnprint((int)Terminal::command.highlighted_buffer_start_pos);
		Terminal::lnprint((int)Terminal::get_pos());
	}

	else if (key == "mkdir") {
		Terminal_commands::__command_mkdir(args);
	}

	else if (key == "cd") {
		Terminal_commands::__command_cddir(args);
	}

	else if (key == "tree") {
		Terminal_commands::__command_tree(args);
	}

	else if (key == "path") {
		Terminal_commands::__command_path(args);
	}

	else if (key == "dir") {
		Terminal_commands::__command_dir(args);
	}

	else {
		lnprint(key + " is invalid command\nenter help for print all commands");
	}
	new_line();
}