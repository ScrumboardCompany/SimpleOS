#include "IRQ/IRQ.h"

using namespace SimpleOS;

void IRQ::init_pic() {
    port_byte_out(0x21, 0xFF);  // Запись в порт данных контроллера PIC1
    port_byte_out(0xA1, 0xFF);  // Запись в порт данных контроллера PIC2

    port_byte_out(0x20, 0x11);  // Команда инициализации (ICW1) для PIC1
    port_byte_out(0xA0, 0x11);  // Команда инициализации (ICW1) для PIC2

    port_byte_out(0x21, 0x20);  // Вектор прерываний для PIC1 (ICW2)
    port_byte_out(0xA1, 0x28);  // Вектор прерываний для PIC2 (ICW2)

    port_byte_out(0x21, 0x04);  // Связывание PIC1 и PIC2 (ICW3 для PIC1)
    port_byte_out(0xA1, 0x02);  // Связывание PIC2 и PIC1 (ICW3 для PIC2)

    port_byte_out(0x21, 0x01);  // Завершение инициализации (ICW4 для PIC1)
    port_byte_out(0xA1, 0x01);  // Завершение инициализации (ICW4 для PIC2)

    port_byte_out(0x21, 0xFD);  // Разрешаем только прерывание IRQ1 (клавиатура)
    port_byte_out(0xA1, 0xFF);  // Отключаем все прерывания для PIC2
}

uint8_t IRQ::port_byte_in(uint16_t port) {
    uint8_t result;

    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));

    return result;
}

void IRQ::port_byte_out(uint16_t port, uint8_t data) {
    __asm__ volatile ("outb %1, %0" : : "dN" (port), "a" (data));
}