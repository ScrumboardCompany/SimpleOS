#pragma once

#ifndef _TERMINAL_

#include "utils/typedef.h"
#include <stdint.h>
#include <stddef.h>

#define VIDEO_MEMORY_ADDRESS 0xB8000

namespace SimpleOS {

	class Terminal {

		uint16_t* buffer = (uint16_t*)0xB8000;
		size_t pos = 0;
	public:

		void print_string(cstr msg);

		void print_char(char c, size_t pos);

		void print_char(char c);
	};
}

#endif // _TERMINAL_