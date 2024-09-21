#pragma once

#ifndef _TERMINAL_

#include "utils/typedef.h"
#include <stdint.h>
#include <stddef.h>

#define VIDEO_MEMORY_ADDRESS 0xB8000

namespace SimpleOS {

	class Terminal {

		static size_t pos;
	public:

		static void print_string(cstr msg);

		static void print_char(char c, size_t pos);

		static void print_char(char c);
	};
}

#endif // _TERMINAL_