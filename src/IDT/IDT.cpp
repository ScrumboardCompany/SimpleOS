#include "IDT/IDT.h"
#include "terminal/terminal.h"

using namespace SimpleOS;

void IDT::init_idt() {

    set_in_idt_slot(0, (uint32_t)dividing_by_zero_handler, 0x08, 0x8E); // 0x08, 0x8E

    load_idt();

    __asm__ __volatile__("sti");
}

void IDT::set_in_idt_slot(int pos, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[pos].offset_first = base & 0xFFFF;
    idt[pos].selector = sel;
    idt[pos].zero = 0;
    idt[pos].type_attr = flags;
    idt[pos].offset_second = (base >> 16) & 0xFFFF;
}

void IDT::load_idt() {
    idt_ptr.limit = (sizeof(IDTSlot) * IDT_SIZE) - 1;
    idt_ptr.base = (uint32_t)&idt;
    //__asm__("lidt %0" : : "m"(idt_ptr));
    __asm__ __volatile__("lidt (%0)" : : "r" (&idt_ptr));
}

extern "C" void SimpleOS::dividing_by_zero() {
    Terminal::print("Failed operation dividing by zero");
    //__asm__ __volatile__("cli; iret");
}

__asm__(
    ".global dividing_by_zero_handler\n"
    "dividing_by_zero_handler:\n"
    "    pusha\n"
    "    call dividing_by_zero\n"
    "    popa\n"
    "    iret\n"
);

IDT::IDTSlot IDT::idt[IDT_SIZE];
IDT::IDTPtr IDT::idt_ptr;