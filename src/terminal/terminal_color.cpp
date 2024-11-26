#include "terminal/terminal.h"

using namespace SimpleOS; 

void Terminal::set_and_fill_terminal_color(Color new_color) {
	set_terminal_color(new_color);
	fill_terminal_color(new_color);
}

void Terminal::fill_terminal_color(Color new_color) {
	char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

	for (size_t i = 0; i < pos; ++i) {
		uint8_t current_color = buffer[i * 2 + 1];
		uint8_t background_color = (current_color >> 4) & 0x0F;

		uint8_t color = ((uint8_t)background_color << 4) | (uint8_t)new_color;
		buffer[i * 2 + 1] = color;
	}
}

void Terminal::set_terminal_color(Color new_color) {
	terminal_color = new_color;
}

void Terminal::set_and_fill_bg_color(Color new_color) {
	set_bg_color(new_color);
	fill_bg_color(new_color);
}

void Terminal::fill_bg_color(Color new_color) {
	char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

	for (size_t i = 0; i < WIDTH * HEIGHT; ++i) {
		uint8_t current_color = buffer[i * 2 + 1];

		uint8_t text_color = current_color & 0x0F;
		uint8_t color = ((uint8_t)new_color << 4) | text_color;

		buffer[i * 2 + 1] = color;
	}
}

void Terminal::set_bg_color(Color new_color) {
	bg_color = new_color;
}

void Terminal::fill_bg_color_at(size_t pos, Color new_color) {
	char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

	uint8_t current_color = buffer[pos * 2 + 1];
	uint8_t text_color = current_color & 0x0F;
	uint8_t color = ((uint8_t)new_color << 4) | text_color;

	buffer[pos * 2 + 1] = color;
}

void Terminal::restore_default_bg_color() {
	char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

	for (size_t i = 0; i < WIDTH * HEIGHT; ++i) {
		uint8_t current_color = buffer[i * 2 + 1];
		uint8_t text_color = current_color & 0x0F;

		uint8_t color = ((uint8_t)bg_color << 4) | text_color;

		buffer[i * 2 + 1] = color;
	}
}

void Terminal::restore_bg_color_at(size_t pos) {
	char* buffer = (char*)VIDEO_MEMORY_ADDRESS;

	uint8_t current_color = buffer[pos * 2 + 1];
	uint8_t text_color = current_color & 0x0F;
	uint8_t color = ((uint8_t)Color::Black << 4) | text_color;

	buffer[pos * 2 + 1] = color;
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
	default: return Color::None;
	}
}

Terminal::Color Terminal::to_color(const string& code) {
	if (code == "black") return Color::Black;
	else if (code == "blue")  return Color::Blue;
	else if (code == "green")  return Color::Green;
	else if (code == "cyan")  return Color::Cyan;
	else if (code == "red")  return Color::Red;
	else if (code == "magenta" || code == "purple")  return Color::Magenta;
	else if (code == "brown")  return Color::Brown;
	else if (code == "grey")  return Color::Grey;
	else if (code == "darkGrey" || code == "dark_grey" || code == "dark-grey")  return Color::DarkGrey;
	else if (code == "lightBlue" || code == "light_blue" || code == "light-blue")  return Color::LightBlue;
	else if (code == "lightGreen" || code == "light_green" || code == "light-green") return Color::LightGreen;
	else if (code == "lightCyan" || code == "light_cyan" || code == "light-cyan") return Color::LightCyan;
	else if (code == "lightRed" || code == "light_red" || code == "light-red") return Color::LightRed;
	else if (code == "lightMagenta" || code == "light_magenta" || code == "light-magenta" || code == "light_purple" || code == "light-purple") return Color::LightMagenta;
	else if (code == "yellow") return Color::Yellow;
	else if (code == "white") return Color::White;
	else return Color::None;
}