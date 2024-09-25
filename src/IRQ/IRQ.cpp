#include "IRQ/IRQ.h"

using namespace SimpleOS;

void IRQ::init_pic() {
    port_byte_out(0x21, 0xFF);  // ������ � ���� ������ ����������� PIC1
    port_byte_out(0xA1, 0xFF);  // ������ � ���� ������ ����������� PIC2

    port_byte_out(0x20, 0x11);  // ������� ������������� (ICW1) ��� PIC1
    port_byte_out(0xA0, 0x11);  // ������� ������������� (ICW1) ��� PIC2

    port_byte_out(0x21, 0x20);  // ������ ���������� ��� PIC1 (ICW2)
    port_byte_out(0xA1, 0x28);  // ������ ���������� ��� PIC2 (ICW2)

    port_byte_out(0x21, 0x04);  // ���������� PIC1 � PIC2 (ICW3 ��� PIC1)
    port_byte_out(0xA1, 0x02);  // ���������� PIC2 � PIC1 (ICW3 ��� PIC2)

    port_byte_out(0x21, 0x01);  // ���������� ������������� (ICW4 ��� PIC1)
    port_byte_out(0xA1, 0x01);  // ���������� ������������� (ICW4 ��� PIC2)

    port_byte_out(0x21, 0xFD);  // ��������� ������ ���������� IRQ1 (����������)
    port_byte_out(0xA1, 0xFF);  // ��������� ��� ���������� ��� PIC2
}

uint8_t IRQ::port_byte_in(uint16_t port) {
    uint8_t result;

    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));

    return result;
}

void IRQ::port_byte_out(uint16_t port, uint8_t data) {
    __asm__ volatile ("outb %1, %0" : : "dN" (port), "a" (data));
}