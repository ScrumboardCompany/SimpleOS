#pragma once

#ifndef _IDT_
#define _IDT_

#include "stdint.h"

#define IDT_SIZE 256

namespace SimpleOS {

    class IDT {

        struct IDTSlot;
        struct IDTPtr;

        static IDTSlot idt[IDT_SIZE];
        static IDTPtr idt_ptr;

    public:

        static void set_in_idt_slot(int pos, uint32_t base, uint16_t sel, uint8_t flags);

		static void init_idt();

        static void register_interrupt_handler(int pos, uint32_t base);

    private:

        static void load_idt();

        struct IDTSlot {
            uint16_t offset_first;
            uint16_t selector;
            uint8_t zero;
            uint8_t type_attr;
            uint16_t offset_second;
        } __attribute__((packed));

        struct IDTPtr {
            uint16_t limit;
            uint32_t base;
        } __attribute__((packed));
    };

    extern "C" void dividing_by_zero();

    //extern "C" void dividing_by_zero_handler();
}

#endif // _IDT_