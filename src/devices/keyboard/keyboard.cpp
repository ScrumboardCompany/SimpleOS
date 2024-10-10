#include "devices/keyboard.h"
#include "Terminal/Terminal.h"
#include "IDT/IDT.h"
#include "libs/io/io.h"

using namespace SimpleOS;

bool Keyboard::is_caps_lock = false;
bool Keyboard::is_console_mode = true;

bool Keyboard::ctrl_pressed = false;
bool Keyboard::shift_pressed = false;
bool Keyboard::alt_pressed = false;

void Keyboard::init_keyboard() {
	IDT::register_interrupt_handler(0x21, (uint32_t)keyboard_handler);
}

extern "C" void keyboard_handler() {
	SimpleOS::Keyboard::__keyboard_handler();
}

void Keyboard::__keyboard_handler() {
	Keyboard::PressedKey key = Keyboard::get_key();

	char c = Keyboard::get_key_char(key);

	switch (key) {
	case Keyboard::PressedKey::Ctrl:
		Keyboard::ctrl_pressed = true;
		break;
	case Keyboard::PressedKey::CtrlReleased:
		Keyboard::ctrl_pressed = false;
		break;
	case Keyboard::PressedKey::Shift:
	case Keyboard::PressedKey::RightShift:
		Keyboard::shift_pressed = true;
		break;
	case Keyboard::PressedKey::ShiftReleased:
	case Keyboard::PressedKey::RightShiftReleased:
		Keyboard::shift_pressed = false;
		break;
	case Keyboard::PressedKey::Alt:
		Keyboard::alt_pressed = true;
		break;
	case Keyboard::PressedKey::AltReleased:
		Keyboard::alt_pressed = false;
		break;
	default:
		break;
	}

	c = (Keyboard::is_caps_lock ? to_upper(c) : to_lower(c));

	Keyboard::__capslock(key);

	if (Keyboard::is_console_mode) {
		Keyboard::__backspace(key);
		Keyboard::__enter(key);
		Keyboard::__arrow_up(key);
		Keyboard::__arrow_down(key);
		Keyboard::__arrow_left(key);
		Keyboard::__arrow_right(key);
	}
	else {
		Keyboard::__textbackspace(key);
		Keyboard::__textenter(key);
		Keyboard::__textctrl(key);
		Keyboard::__textarrow_up(key);
		Keyboard::__textarrow_down(key);
		Keyboard::__arrow_left(key);
		Keyboard::__arrow_right(key);
	}

	if (c) {
		
		if (key == Keyboard::PressedKey::C && ctrl_pressed) {
			if (Terminal::command.highlighted_buffer != "") {
				Terminal::command.clipboard = Terminal::command.highlighted_buffer;
				Terminal::clear_highlighted_buffer();
			}
		}
		else if (key == Keyboard::PressedKey::V && ctrl_pressed) {
			Terminal::command.buffer.push(Terminal::command.clipboard, Terminal::command.buffer_pos - 1);
			Terminal::clear_highlighted_buffer();

			Terminal::print(Terminal::command.clipboard);
		}
		else {
			if (!Terminal::command.highlighted_buffer.empty()) Terminal::delete_highlighted_text();
			Terminal::command.buffer.push(c, Keyboard::is_console_mode ? Terminal::get_buffer_pos() - 1 : Terminal::get_pos());
			Keyboard::is_console_mode ? Terminal::input_print(c) : Terminal::text_print(c);
		}

		Keyboard::reset_selected_command_pos();
	}

	outb(0x20, 0x20);
	//__asm__ volatile("sti");
}

Keyboard::PressedKey Keyboard::get_key() {
	return (PressedKey)(inb(0x60));


}

char Keyboard::get_key_char(Keyboard::PressedKey key) {
	switch (key) {
	case Keyboard::PressedKey::A: return 'A';
	case Keyboard::PressedKey::B: return 'B';
	case Keyboard::PressedKey::C: return 'C';
	case Keyboard::PressedKey::D: return 'D';
	case Keyboard::PressedKey::E: return 'E';
	case Keyboard::PressedKey::F: return 'F';
	case Keyboard::PressedKey::G: return 'G';
	case Keyboard::PressedKey::H: return 'H';
	case Keyboard::PressedKey::I: return 'I';
	case Keyboard::PressedKey::J: return 'J';
	case Keyboard::PressedKey::K: return 'K';
	case Keyboard::PressedKey::L: return 'L';
	case Keyboard::PressedKey::M: return 'M';
	case Keyboard::PressedKey::N: return 'N';
	case Keyboard::PressedKey::O: return 'O';
	case Keyboard::PressedKey::P: return 'P';
	case Keyboard::PressedKey::Q: return 'Q';
	case Keyboard::PressedKey::R: return 'R';
	case Keyboard::PressedKey::S: return 'S';
	case Keyboard::PressedKey::T: return 'T';
	case Keyboard::PressedKey::U: return 'U';
	case Keyboard::PressedKey::V: return 'V';
	case Keyboard::PressedKey::W: return 'W';
	case Keyboard::PressedKey::X: return 'X';
	case Keyboard::PressedKey::Y: return 'Y';
	case Keyboard::PressedKey::Z: return 'Z';
	case Keyboard::PressedKey::Num0: return '0';
	case Keyboard::PressedKey::Num1: return '1';
	case Keyboard::PressedKey::Num2: return '2';
	case Keyboard::PressedKey::Num3: return '3';
	case Keyboard::PressedKey::Num4: return '4';
	case Keyboard::PressedKey::Num5: return '5';
	case Keyboard::PressedKey::Num6: return '6';
	case Keyboard::PressedKey::Num7: return '7';
	case Keyboard::PressedKey::Num8: return '8';
	case Keyboard::PressedKey::Num9: return '9';
	case Keyboard::PressedKey::Space: return ' ';
	case Keyboard::PressedKey::Tab: return '\t';
	case Keyboard::PressedKey::Equals: return '=';
	case Keyboard::PressedKey::Minus: return '-';
	case Keyboard::PressedKey::Slash: return '/';
	case Keyboard::PressedKey::Backslash: return '\\';
	case Keyboard::PressedKey::Semicolon: return ';';
	case Keyboard::PressedKey::Apostrophe: return '\'';
	case Keyboard::PressedKey::Comma: return ',';
	case Keyboard::PressedKey::Period: return '.';
	case Keyboard::PressedKey::LeftBracket: return '[';
	case Keyboard::PressedKey::RightBracket: return ']';
	case Keyboard::PressedKey::Tilde: return '`';

	default: return _NO_CHAR;
	}
}

void Keyboard::reset_selected_command_pos() {
	if (!Terminal::command.commands.empty()) {
		Terminal::command.selected_command_pos = Terminal::command.commands.size();
	}
	else Terminal::command.selected_command_pos = -1;
}

void Keyboard::__handle_arrow(bool isUp) {
	if (!Terminal::command.commands.empty() && Terminal::command.selected_command_pos) {
		if (isUp && Terminal::command.selected_command_pos) {
			if (Terminal::command.selected_command_pos == 0) return;
			--Terminal::command.selected_command_pos;
		}
		else {
			if (Terminal::command.selected_command_pos < Terminal::command.commands.size() - 1)
				++Terminal::command.selected_command_pos;

			else return;
		}
	}

	Terminal::command.buffer = Terminal::command.commands[Terminal::command.selected_command_pos];

	Terminal::delete_line();
	Terminal::print('>');
	Terminal::print(Terminal::command.buffer);
}

void Keyboard::__texthandle_arrow(bool isUp) {
	if (isUp && Terminal::get_pos() > 80) {
		Terminal::set_pos(Terminal::get_pos() - 80);
	} else if (!isUp) {
		Terminal::set_pos(Terminal::get_pos() + 80);
	}
}

void Keyboard::change_mode(bool is_console_mode) {
	Terminal::command.buffer = "";
	Keyboard::reset_selected_command_pos();

	Keyboard::is_console_mode = is_console_mode;
}