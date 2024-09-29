#include "devices/keyboard.h"
#include "terminal/terminal.h"

using namespace SimpleOS;

void Keyboard::__backspace(PressedKey key) {
	if (key == Keyboard::PressedKey::Backspace) {
		Terminal::delete_char(Terminal::get_pos());
		Keyboard::buffer = pop_char(Keyboard::buffer);
		--Keyboard::buffer_size;

		Keyboard::reset_selected_command_pos();
	}
}

void Keyboard::__enter(PressedKey key) {
	if (key == Keyboard::PressedKey::Enter) {
		if (Keyboard::buffer_size > 0) {
			Terminal::execute_command(Keyboard::buffer);

			char* buffer_copy = (char*)malloc(strlen(Keyboard::buffer) + 1);
			strcpy(buffer_copy, Keyboard::buffer);

			Keyboard::commands.push(buffer_copy);
			Keyboard::selected_command_pos = Keyboard::commands.size();

			Keyboard::buffer = (char*)malloc(1);
			Keyboard::buffer[0] = '\0';
			Keyboard::buffer_size = 0;
		}
		else Terminal::print("Buffer size small");
		Terminal::new_line();

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