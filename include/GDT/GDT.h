#pragma once

#ifndef _GDT_
#define _GDT_

#include <stdint.h>

namespace SimpleOS {

	class GDT {

		struct GDTSlot;
		struct GDTPtr;
		struct tss_entry_t;

		static GDTSlot gdt[6];
		static GDTPtr gdt_ptr;
        static tss_entry_t tss_entry;

	public:

		static void init_gdt();

	private:

		static void set_in_gdt_slot(int pos, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran);

		static void load_gdt();

		static void load_tss();

		static void write_tss(int32_t pos, uint16_t ss0, uint32_t esp0);

		struct GDTSlot {
			uint16_t limit_low;
			uint16_t base_low;
			uint8_t base_middle;
			uint8_t access;
			uint8_t granularity;
			uint8_t base_high;
		} __attribute__((packed));

		struct GDTPtr {
			uint16_t limit;
			uint32_t base;
		} __attribute__((packed));

		struct tss_entry_t {
            uint32_t    prev_tss;
            uint32_t    esp0;
            uint32_t    ss0;
            uint32_t    esp1;
            uint32_t    ss1;
            uint32_t    esp2;
            uint32_t    ss2;
            uint32_t    cr3;
            uint32_t    eip;
            uint32_t    eflags;
            uint32_t    eax;
            uint32_t    ecx;
            uint32_t    edx;
            uint32_t    ebx;
            uint32_t    esp;
            uint32_t    ebp;
            uint32_t    esi;
            uint32_t    edi;
            uint32_t    es;
            uint32_t    cs;
            uint32_t    ss;
            uint32_t    ds;
            uint32_t    fs;
            uint32_t    gs;
            uint32_t    ldt;
            uint16_t    trap;
            uint16_t    iomap_base;
        } __attribute__ ((packed));

	};
}

#endif // _GDT_