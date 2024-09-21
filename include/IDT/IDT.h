#pragma once

#ifndef _IDT_

#include "stdint.h"

#define IDT_SIZE 256

namespace SimpleOS {

    class IDT {

        struct IDTSlot;
        struct IDTPtr;

        static IDTSlot idt[IDT_SIZE];
        static IDTPtr idt_ptr;

    public:

        static void init_idt();

    private:
        
        static void dividing_by_zero();

    private:

        static void set_in_idt_slot(int pos, uint32_t base, uint16_t sel, uint8_t flags);

        static void load_idt();

        struct IDTSlot {
            uint16_t offset_first;
            uint16_t selector;
            uint8_t zero;
            uint8_t type_attr;
            uint16_t offset_second;
        };

        struct IDTPtr {
            uint16_t limit;
            uint32_t base;
        };
    };
}

#endif // _IDT_