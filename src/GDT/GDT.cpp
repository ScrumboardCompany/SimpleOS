#include "GDT/GDT.h"
#include "utils/utils.h"


using namespace SimpleOS;

void GDT::init_gdt() {
	gdt_ptr.limit = (sizeof(struct GDTSlot) * 6) - 1;
	gdt_ptr.base = (unsigned int)&gdt;

	set_in_gdt_slot(0, 0, 0, 0, 0);

	set_in_gdt_slot(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

	set_in_gdt_slot(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

	set_in_gdt_slot(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);

	set_in_gdt_slot(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);
	write_tss(5, 0x10, 0x0);

	load_gdt();
	load_tss();
}

void GDT::set_in_gdt_slot(int pos, uint64_t base, uint64_t limit, uint8_t access, uint8_t gran) {
	gdt[pos].base_low = (base & 0xFFFF);
	gdt[pos].base_middle = (base >> 16) & 0xFF;
	gdt[pos].base_high = (base >> 24) & 0xFF;
	gdt[pos].limit_low = (limit & 0xFFFF);
	gdt[pos].granularity = (limit >> 16) & 0X0F;
	gdt[pos].granularity |= (gran & 0xF0);
	gdt[pos].access = access;
}

void GDT::write_tss(int32_t pos, uint16_t ss0, uint32_t esp0) {
	uintptr_t base = (uintptr_t)&tss_entry;
	uintptr_t limit = base + sizeof(tss_entry);

	set_in_gdt_slot(pos, base, limit, 0xE9, 0x00);

	memset(&tss_entry, 0x0, sizeof(tss_entry));

	tss_entry.ss0 = ss0;
	tss_entry.esp0 = esp0;

	tss_entry.cs = 0x0b;
	tss_entry.ss =
		tss_entry.ds =
		tss_entry.es =
		tss_entry.fs =
		tss_entry.gs = 0x13;
	tss_entry.iomap_base = sizeof(tss_entry);
}

GDT::GDTSlot GDT::gdt[6];
GDT::GDTPtr GDT::gdt_ptr;
GDT::tss_entry_t GDT::tss_entry;