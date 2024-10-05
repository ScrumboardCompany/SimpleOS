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
#include "fs/fs.h"
#include "graphics/graphics.h"

using namespace SimpleOS;

//void draw_square(int x, int y, int size, uint8_t color) {
//    for (int i = 0; i < size; ++i) {
//        for (int j = 0; j < size; ++j) {
//            Video::draw_pixel(x + j, y + i, color);
//        }
//    }
//}
//
//void test_direct_memory_access() {
//    uint8_t* video_memory = (uint8_t*)_GRAP_VIDEO_MEMORY_ADDRESS;
//
//    video_memory[0] = 0x0F;
//    video_memory[100] = 0x1F;
//    video_memory[64000] = 0x2F;
//}
//
//void fill_screen(uint8_t color) {
//    uint8_t* video_memory = (uint8_t*)_GRAP_VIDEO_MEMORY_ADDRESS;
//
//    for (int i = 0; i < 320 * 200; i++) {
//        video_memory[i] = color;
//    }
//}

extern "C" void kernel_main(void) {
    GDT::init_gdt();
    IDT::init_idt();
    IRQ::init_pic();
    Keyboard::init_keyboard();
    FileSystem::init_fs();

    Terminal::print('>');

    while (true) {
        __asm__ volatile("sti");
        __asm__ volatile("hlt");
    }
}