#pragma once

#ifndef _TERMINAL_COMMANDS_
#define _TERMINAL_COMMANDS_

#include "utils/typedef.h"

namespace SimpleOS {

	class Terminal_commands {
	public:
		static void __command_color(char** args);
		static void __command_fill(char** args);
		static void __command_set(char** args);

		static void __command_clear(char** args);
		static void __command_reset(char** args);
		static void __command_echo(char** args);
		static void __command_help(char** args);
		static void __command_cat(char** args);

		static void __command_mkfile(char** args);
		static void __command_rdfile(char** args);
		static void __command_rmfile(char** args);
		static void __command_wrfile(char** args);
		static void __command_apfile(char** args);
		static void __command_exfile(char** args);
		static void __command_format(char** args);

		static void __command_open(char** args);

		static void __command_time(char** args);

		static bool __check_argc(char** args, size_t argc);
	};
}

#endif // _TERMINAL_COMMANDS_