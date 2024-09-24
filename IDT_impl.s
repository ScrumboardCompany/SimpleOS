.global _ZN8SimpleOS3IDT8load_idtEv
.extern _ZN8SimpleOS3IDT7idt_ptrE
.section .text
_ZN8SimpleOS3IDT8load_idtEv:
	lidt [_ZN8SimpleOS3IDT7idt_ptrE]
	ret