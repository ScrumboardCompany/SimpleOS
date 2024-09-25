#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

#if !defined(__i386__)
#error "This OS needs to be compiled with a ix86-elf compiler"
#endif

#include "terminal/terminal.h"
#include "IDT/IDT.h"
#include "GDT/GDT.h"
#include "IRQ/IRQ.h"
#include "devices/keyboard.h"

using namespace SimpleOS;

extern "C" void kernel_main(void) {
    GDT::init_gdt();
    IDT::init_idt();
    IRQ::init_pic();
    Keyboard::init_keyboard();

    Terminal::print(">");

    while (true) {
        __asm__ volatile("sti");
        __asm__ volatile("hlt");
    }
}