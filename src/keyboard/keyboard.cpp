#include "devices/keyboard.h"
#include "IDT/IDT.h"
#include "Terminal/Terminal.h"
#include "IRQ/IRQ.h"

using namespace SimpleOS;

string Keyboard::buffer = nullptr;

bool Keyboard::is_caps_lock = false;

vector<string> Keyboard::commands;
int Keyboard::selected_command_pos = -1;

void Keyboard::init_keyboard() {
    IDT::register_interrupt_handler(0x21, (uint32_t)keyboard_handler);
}

extern "C" void SimpleOS::keyboard_handler() {
	Keyboard::PressedKey key = Keyboard::get_key();

	char c = Keyboard::get_key_char(key);

	c = (Keyboard::is_caps_lock ? to_upper(c) : to_lower(c));

	if (c) {
		Keyboard::buffer.push(c);
		Terminal::print(c);

		Keyboard::reset_selected_command_pos();
	}

	Keyboard::__backspace(key);
	Keyboard::__enter(key);
	Keyboard::__capslock(key);
	Keyboard::__arrow_up(key);
	Keyboard::__arrow_down(key);	

	IRQ::port_byte_out(0x20, 0x20);
}

Keyboard::PressedKey Keyboard::get_key() {
    return static_cast<PressedKey>(IRQ::port_byte_in(0x60));
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
	default: return _NO_CHAR;
	}
}

void Keyboard::reset_selected_command_pos() {
	if (!Keyboard::commands.empty()) {
		Keyboard::selected_command_pos = Keyboard::commands.size();
	}
	else Keyboard::selected_command_pos = -1;
}

void Keyboard::__handle_arrow(bool isUp) {
	if (!Keyboard::commands.empty() && Keyboard::selected_command_pos > 0) {
		isUp ? ++Keyboard::selected_command_pos : --Keyboard::selected_command_pos;

		Keyboard::buffer = Keyboard::commands[Keyboard::selected_command_pos];

		Terminal::delete_line();
		Terminal::print('>');
		Terminal::print(Keyboard::buffer);
	}
}