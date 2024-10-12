#pragma once

#ifndef _TERMINAL_COMMANDS_
#define _TERMINAL_COMMANDS_

#include "utils/typedef.h"
#include "libs/vector/vector.h"
#include "libs/string/class.h"

namespace SimpleOS {

	class Terminal_commands {
	public:
		static void __command_color(vector<string>& args);
		static void __command_fill(vector<string>& args);
		static void __command_set(vector<string>& args);

		static void __command_clear(vector<string>& args);
		static void __command_reset(vector<string>& args);
		static void __command_echo(vector<string>& args);
		static void __command_help(vector<string>& args);
		static void __command_cat(vector<string>& args);

		static void __command_mkfile(vector<string>& args);
		static void __command_rdfile(vector<string>& args);
		static void __command_rmfile(vector<string>& args);
		static void __command_wrfile(vector<string>& args);
		static void __command_apfile(vector<string>& args);
		static void __command_exfile(vector<string>& args);
		static void __command_format(vector<string>& args);

		static void __command_mkdir(vector<string>& args);
		static void __command_rmdir(vector<string>& args);
		static void __command_cddir(vector<string>& args);
		static void __command_exdir(vector<string>& args);
		static void __command_tree(vector<string>& args);
		static void __command_path(vector<string>& args);
		static void __command_dir(vector<string>& args);

		static void __command_open(vector<string>& args);

		static void __command_time(vector<string>& args);

		static bool __check_argc(vector<string>& args, size_t argc, bool print_error = true);
	};
}

#endif // _TERMINAL_COMMANDS_