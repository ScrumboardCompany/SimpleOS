#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This OS needs to be compiled with a ix86-elf compiler"
#endif

#include "terminal/terminal.h"

using namespace SimpleOS;

extern "C" void kernel_main(void) {
	Terminal terminal;

	terminal.print_string("Hello from SimpleOS");
}