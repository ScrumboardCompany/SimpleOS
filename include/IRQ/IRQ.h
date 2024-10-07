#pragma once

#ifndef _IRQ_
#define _IRQ_

#include <stdint.h>

namespace SimpleOS {

	class IRQ {

	public:
		static void init_pic();
	};

}

#endif // _IRQ_