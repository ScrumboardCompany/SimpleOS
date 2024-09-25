#include "IDT/IDT.h"
#include "terminal/terminal.h"
#include "utils/utils.h"

using namespace SimpleOS;

void IDT::init_idt() {
    idt_ptr.limit = (sizeof(struct IDTSlot) * IDT_SIZE) - 1;
    idt_ptr.base = (uintptr_t)&idt;

    memset(&idt, 0, sizeof(struct IDTSlot) * IDT_SIZE);
    
    load_idt();

    set_in_idt_slot(0x00, (uint32_t)dividing_by_zero, 0x08, 0x8E);
}

void IDT::register_interrupt_handler(int pos, uint32_t base) {
    set_in_idt_slot(pos, base, 0x08, 0x8E);
}

void IDT::set_in_idt_slot(int pos, uint32_t base, uint16_t sel, uint8_t flags) {
    //idt[pos].offset_first = base & 0xFFFF;
    //idt[pos].selector = sel;
    //idt[pos].zero = 0;
    //idt[pos].type_attr = flags | 0x60;
    //idt[pos].offset_second = (base >> 16) & 0xFFFF;
    idt[pos].offset_first = base & 0xFFFF;
    idt[pos].selector = sel;
    idt[pos].zero = 0;
    idt[pos].type_attr = flags;
    idt[pos].offset_second = (base >> 16) & 0xFFFF;
}

extern "C" void SimpleOS::dividing_by_zero() {
    Terminal::print("Failed operation dividing by zero");
}

IDT::IDTSlot IDT::idt[IDT_SIZE];
IDT::IDTPtr IDT::idt_ptr;