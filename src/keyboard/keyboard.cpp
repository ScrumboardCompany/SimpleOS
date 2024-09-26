#include "devices/keyboard.h"
#include "IDT/IDT.h"
#include "Terminal/Terminal.h"
#include "IRQ/IRQ.h"

using namespace SimpleOS;

void Keyboard::init_keyboard() {
    IDT::register_interrupt_handler(0x21, (uint32_t)keyboard_handler);
	Keyboard::buffer[0] = '\0';
}

extern "C" void SimpleOS::keyboard_handler() {
	Keyboard::PressedKey key = Keyboard::get_key();

	char c = Keyboard::get_key_char(key);

	if (Keyboard::is_caps_lock)
		c = to_upper(c);
	else c = to_lower(c);

	if (c) {
		Keyboard::buffer = add_char(Keyboard::buffer, c);
		++Keyboard::buffer_size;
		Terminal::print(c);
	}

	if (key == Keyboard::PressedKey::Enter) {
		if (Keyboard::buffer_size > 0) {
			Terminal::execute_command(Keyboard::buffer);

			Keyboard::buffer = (char*)malloc(1);
			Keyboard::buffer[0] = '\0';
			Keyboard::buffer_size = 0;
		}
		else Terminal::print("Buffer size small");
		Terminal::new_line();
	}

	if (key == Keyboard::PressedKey::CapsLock) {
		Keyboard::is_caps_lock = !Keyboard::is_caps_lock;
	}

	if (key == Keyboard::PressedKey::Backspace) {
		Terminal::delete_char(Terminal::get_pos());
		Keyboard::buffer = pop_char(Keyboard::buffer);
	}

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

char* Keyboard::buffer = (char*)malloc(1);
SimpleOS::size_t Keyboard::buffer_size = 0;
bool Keyboard::is_caps_lock = false;