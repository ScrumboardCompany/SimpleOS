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
	};

}

#endif // _IRQ_