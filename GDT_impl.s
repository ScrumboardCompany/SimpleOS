.global _ZN8SimpleOS3GDT8load_gdtEv
.extern _ZN8SimpleOS3GDT7gdt_ptrE
_ZN8SimpleOS3GDT8load_gdtEv:
	lgdt _ZN8SimpleOS3GDT7gdt_ptrE

.global _ZN8SimpleOS3GDT8load_tssEv
_ZN8SimpleOS3GDT8load_tssEv:
    mov %ax, 0x28
    ltr %ax
    ret
