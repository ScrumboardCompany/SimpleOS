#include "IRQ/IRQ.h"
#include "libs/io/io.h"

using namespace SimpleOS;

void IRQ::init_pic() {
    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);

    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    outb(0x21, 0x20);
    outb(0xA1, 0x28);

    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    outb(0x21, 0xFD);
    outb(0xA1, 0xFF);
}

