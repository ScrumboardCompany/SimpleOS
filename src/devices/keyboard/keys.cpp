#include "devices/keyboard.h"
#include "terminal/terminal.h"
#include "fs/fs.h"


using namespace SimpleOS;

void Keyboard::__backspace(PressedKey key) {
	if (key == Keyboard::PressedKey::Backspace) {
		if (Terminal::get_pos() % WIDTH > 1) {
			Terminal::delete_char(Terminal::get_pos());

			//Keyboard::buffer.pop();
			Keyboard::buffer.pop(Terminal::get_buffer_pos() - 1);
			Keyboard::reset_selected_command_pos();
		}
	}
}

void Keyboard::__textbackspace(PressedKey key) {
	if (key == Keyboard::PressedKey::Backspace) {
		if (Terminal::get_pos() > 0) {
			Terminal::delete_char(Terminal::get_pos());

			Keyboard::buffer.pop(Terminal::get_pos());
		}
	}
}

void Keyboard::__enter(PressedKey key) {
	if (key == Keyboard::PressedKey::Enter) {
		if (Keyboard::buffer.size() > 0) {
			Terminal::execute_command(Keyboard::buffer.c_str());

			Keyboard::commands.push(Keyboard::buffer);
			Keyboard::selected_command_pos = Keyboard::commands.size();

			if (is_console_mode) {
				Keyboard::buffer = "";
				Terminal::print('>');
			}
		}

		Keyboard::reset_selected_command_pos();
	}
}

void Keyboard::__textenter(PressedKey key) {
	if (key == Keyboard::PressedKey::Enter) {
		buffer.push('\n');
		Terminal::new_line();
	}
}

void Keyboard::__capslock(PressedKey key) {
	if (key == Keyboard::PressedKey::CapsLock) {
		Keyboard::is_caps_lock = !Keyboard::is_caps_lock;
	}
}

void Keyboard::__textctrl(PressedKey key) {
	if (key == Keyboard::PressedKey::Ctrl) {

		//Terminal::lnprint("Ctrl + x good");
		string opened_file = FileSystem::get_opened_file();
		//Terminal::lnprint("opened file: ");
		//Terminal::print(opened_file);
		string temp;

		if (opened_file != "") {
			//Terminal::lnprint("good opened file");

			FileSystem::write_to_file(opened_file.c_str(), buffer.c_str());
			temp = buffer;

			change_mode(true);
			FileSystem::close_file();
			Terminal::clear();
		}
		Terminal::print('>');

		Terminal::print(temp);
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

void Keyboard::__arrow_left(PressedKey key) {
	if (key == Keyboard::PressedKey::ArrowLeft) {
		if (Terminal::get_buffer_pos() > 1) {
			Terminal::set_pos(Terminal::get_pos() - 1);
			Terminal::set_buffer_pos(Terminal::get_buffer_pos() - 1);
			Terminal::move_cursor(Terminal::get_pos());
		}
	}
}

void Keyboard::__arrow_right(PressedKey key) {
	if (key == Keyboard::PressedKey::ArrowRight) {
		if (Terminal::get_pos() < Keyboard::buffer.size() + 1) {
			Terminal::set_pos(Terminal::get_pos() + 1);
			Terminal::set_buffer_pos(Terminal::get_buffer_pos() + 1);
			Terminal::move_cursor(Terminal::get_pos());
		}
	}
}