#include "terminal/terminal.h"

using namespace SimpleOS; 

void Terminal::set_and_fill_terminal_color(Color new_color) {
	fill_terminal_color(new_color);
	set_terminal_color(new_color);
}

void SimpleOS::Terminal::fill_terminal_color(Color new_color) {
	char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

	for (size_t i = 0; i < pos; ++i) {
		buffer[i * 2 + 1] = (uint8_t)new_color;
	}
}

void SimpleOS::Terminal::set_terminal_color(Color new_color) {
	terminal_color = new_color;
}

Terminal::Color Terminal::to_color(int code) {
	switch (code) {
	case 0:  return Color::Black;
	case 1:  return Color::Blue;
	case 2:  return Color::Green;
	case 3:  return Color::Cyan;
	case 4:  return Color::Red;
	case 5:  return Color::Magenta;
	case 6:  return Color::Brown;
	case 7:  return Color::Grey;
	case 8:  return Color::DarkGrey;
	case 9:  return Color::LightBlue;
	case 10: return Color::LightGreen;
	case 11: return Color::LightCyan;
	case 12: return Color::LightRed;
	case 13: return Color::LightMagenta;
	case 14: return Color::Yellow;
	case 15: return Color::White;
	default: return Color::Grey;
	}
}