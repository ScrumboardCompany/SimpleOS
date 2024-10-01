#include "devices/keyboard.h"
#include "terminal/terminal.h"

using namespace SimpleOS;

void Keyboard::__backspace(PressedKey key) {
	if (key == Keyboard::PressedKey::Backspace) {
		if (Terminal::get_pos() % WIDTH > 1) {
			Terminal::delete_char(Terminal::get_pos());

			Keyboard::buffer.pop();
			Keyboard::reset_selected_command_pos();
		}
	}
}

void Keyboard::__enter(PressedKey key) {
	if (key == Keyboard::PressedKey::Enter) {
		if (Keyboard::buffer.size() > 0) {
			Terminal::execute_command(Keyboard::buffer.c_str());

			Keyboard::commands.push(Keyboard::buffer);
			Keyboard::selected_command_pos = Keyboard::commands.size();

			Keyboard::buffer = "";
			Terminal::print('>');
		}

		Keyboard::reset_selected_command_pos();
	}
}

void Keyboard::__capslock(PressedKey key) {
	if (key == Keyboard::PressedKey::CapsLock) {
		Keyboard::is_caps_lock = !Keyboard::is_caps_lock;
	}
}

void Keyboard::__arrow_up(PressedKey key) {
	if (key == Keyboard::PressedKey::ArrowUp) {
		__handle_arrow(true);
	}
}

void Keyboard::__arrow_down(PressedKey key) {
	if (key == Keyboard::PressedKey::ArrowDown) {
		__handle_arrow(false);
	}
}