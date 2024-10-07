#pragma once

#ifndef _IO_
#define _IO_

#include "utils/typedef.h"

u8 inb(u16 port);

void outb(u16 port, u8 data);

u16 inw(u16 port);

void outw(u16 port, u16 data);

uint32_t inl(u16 port);

void outl(u16 port, u32 data);

#endif // _IO_