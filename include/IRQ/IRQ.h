#pragma once

#ifndef _IRQ_
#define _IRQ_

#include <stdint.h>

namespace SimpleOS {

	class IRQ {

	public:
		static void init_pic();

		static uint8_t port_byte_in(uint16_t port);

		static void port_byte_out(uint16_t port, uint8_t data);

		static void outw(uint16_t port, uint16_t data);

		static uint16_t inw(uint16_t port);
	};

}

#endif // _IRQ_