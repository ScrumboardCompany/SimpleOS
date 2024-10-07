#include "libs/io/io.h"

u8 inb(u16 port) {
    u8 result;

    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));

    return result;
}

void outb(u16 port, u8 data) {
    __asm__ volatile ("outb %1, %0" : : "dN" (port), "a" (data));
}

u16 inw(u16 port) {
    u16 result;
    __asm__ volatile ("inw %1, %0"
        : "=a"(result)
        : "dN"(port));
    return result;
}

void outw(u16 port, u16 data) {
    __asm__ volatile ("outw %0, %1"
        :
    : "a"(data), "dN"(port));
}

u32 inl(u16 port) {
    u32 result;
    __asm__ __volatile__("inl %1, %0" : "=a"(result) : "dN"(port));
    return result;
}

void outl(u16 port, u32 data) {
    __asm__ __volatile__("outl %0, %1" : : "a"(data), "dN"(port));
}
