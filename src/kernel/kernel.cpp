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
#include "fs/disk.h"

using namespace SimpleOS;

extern "C" void kernel_main(void) {
    GDT::init_gdt();
    IDT::init_idt();
    IRQ::init_pic();
    Keyboard::init_keyboard();
    Terminal::print('>');

    ata_write_to_sector(0, "Hello ");
    ata_append_to_sector(0, "SimpleOS!");

    char buffer3[512];
    ata_read_sector(0, buffer3);

    Terminal::print(buffer3);

    while (true) {
        __asm__ volatile("sti");
        __asm__ volatile("hlt");
    }
}