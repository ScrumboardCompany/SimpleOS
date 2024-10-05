#include "graphics/graphics.h"
#include "IDT/IDT.h"
#include "IRQ/IRQ.h"

using namespace SimpleOS;

void Video::set_vga_mode() {
    IRQ::port_byte_out(0x3C2, 0x63);

    IRQ::port_byte_out(0x3D4, 0x11);
    IRQ::port_byte_out(0x3D5, 0x80);

    static unsigned char g_320x200x256[] = {
        0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F, 0x00, 0x41,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x0E, 0x8F, 0x28,
        0x40, 0x96, 0xB9, 0xA3, 0xFF
    };

    for (size_t i = 0; i < sizeof(g_320x200x256); i++) {
        IRQ::port_byte_out(0x3D4, i);
        IRQ::port_byte_out(0x3D5, g_320x200x256[i]);
    }

    IRQ::port_byte_out(0x3CE, 0x00);
    IRQ::port_byte_out(0x3CF, 0x00);

    IRQ::port_byte_out(0x3CE, 0x01);
    IRQ::port_byte_out(0x3CF, 0x00);

    IRQ::port_byte_out(0x3CE, 0x05);
    IRQ::port_byte_out(0x3CF, 0x40);

    IRQ::port_byte_out(0x3CE, 0x06);
    IRQ::port_byte_out(0x3CF, 0x05);

    for (int i = 0; i < 16; i++) {
        IRQ::port_byte_out(0x3C0, i);
        IRQ::port_byte_out(0x3C0, i);
    }
    IRQ::port_byte_out(0x3C0, 0x20);
}

void Video::draw_pixel(int x, int y, uint8_t color) {
    if (x < 0 || x >= 320 || y < 0 || y >= 200) return;
    uint8_t* video_memory = (uint8_t*)_GRAP_VIDEO_MEMORY_ADDRESS;
    int offset = y * 320 + x;
    video_memory[offset] = color;
}