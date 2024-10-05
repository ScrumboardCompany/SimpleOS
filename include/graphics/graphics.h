#pragma once

#ifndef _GRAPHICS_
#define _GRAPHICS_

#include "utils/typedef.h"

#define _GRAP_VIDEO_MEMORY_ADDRESS 0xA0000

namespace SimpleOS {

	class Video {
	public:
		static void set_vga_mode();

		static void draw_pixel(int x, int y, uint8_t color);
	};
}

#endif // _GRAPHICS_