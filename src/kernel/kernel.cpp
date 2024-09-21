#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This OS needs to be compiled with a ix86-elf compiler"
#endif

#include "terminal/terminal.h"
#include "IDT/IDT.h"

using namespace SimpleOS;

extern "C" void kernel_main(void) {
	IDT::init_idt();
    Terminal::print_char('D');
    Terminal::print_char('A');
	Terminal::print_string("Hello from SimpleOS");
    Terminal::print_string("Hello from SimpleOS2");
}