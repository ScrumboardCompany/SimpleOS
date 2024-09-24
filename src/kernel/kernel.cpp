#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This OS needs to be compiled with a ix86-elf compiler"
#endif

#include "terminal/terminal.h"
#include "IDT/IDT.h"
#include "GDT/GDT.h"

using namespace SimpleOS;

extern "C" void kernel_main(void) {
	GDT::init_gdt();
	IDT::init_idt();

	Terminal::print("Hello SimpleOS");

	__asm__ __volatile__("int $0");
}