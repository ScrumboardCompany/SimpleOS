#include "devices/keyboard.h"
#include "terminal/terminal.h"
#include "fs/fs.h"
#include "utils/utils.h"

using namespace SimpleOS;

void Keyboard::__backspace(PressedKey key) {
	if (key == Keyboard::PressedKey::Backspace) {
		if (!Terminal::command.highlighted_buffer.empty()) {


			Terminal::delete_highlighted_text();

		}
		else if (Terminal::get_pos() % WIDTH > 1) {
			Terminal::delete_char(Terminal::get_pos());
			Terminal::command.buffer.pop(Terminal::get_buffer_pos() - 1);
			Keyboard::reset_selected_command_pos();
		}
	}
}

void Keyboard::__textbackspace(PressedKey key) {
	if (key == Keyboard::PressedKey::Backspace) {
		if (Terminal::get_pos() > 0) {
			if (!Terminal::command.highlighted_buffer.empty()) {
				Terminal::delete_highlighted_text();
			}
			else {
				Terminal::textdelete_char(Terminal::get_pos());

				Terminal::command.buffer.pop(Terminal::get_pos());
			}
		}
	}
}

void Keyboard::__enter(PressedKey key) {
	if (key == Keyboard::PressedKey::Enter) {
		if (Terminal::command.buffer.size() > 0) {
			Terminal::execute_command(Terminal::command.buffer.c_str());

			Terminal::command.commands.push(Terminal::command.buffer);
			Terminal::command.selected_command_pos = Terminal::command.commands.size();

			if (is_console_mode) {
				Terminal::command.buffer = "";
				Terminal::print('>');
			}

			Terminal::current_line++;
		}

		Terminal::clear_highlighted_buffer();
		Terminal::restore_default_bg_color();

		Keyboard::reset_selected_command_pos();
	}
}

void Keyboard::__textenter(PressedKey key) {
	if (key == Keyboard::PressedKey::Enter) {
		Terminal::command.buffer.push('\n');
		Terminal::new_line();
		Terminal::current_line++;
		Terminal::clear_highlighted_buffer();
		Terminal::restore_default_bg_color();
	}
}

void Keyboard::__capslock(PressedKey key) {
	if (key == Keyboard::PressedKey::CapsLock) {
		Keyboard::is_caps_lock = !Keyboard::is_caps_lock;
	}
}

void Keyboard::__textctrl(PressedKey key) {
	if (key == Keyboard::PressedKey::X && ctrl_pressed) {

		string opened_file = FileSystem::get_opened_file();
		string temp;

		if (opened_file != "") {

			FileSystem::write_to_file(opened_file.c_str(), Terminal::command.buffer.c_str());

			change_mode(true);
			FileSystem::close_file();
			Terminal::clear();
		}
		Terminal::print('>');

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

void Keyboard::__textarrow_up(PressedKey key) {
	if (key == Keyboard::PressedKey::ArrowUp) {
		__texthandle_arrow(true);
	}
}

void Keyboard::__textarrow_down(PressedKey key) {
	if (key == Keyboard::PressedKey::ArrowDown) {
		__texthandle_arrow(false);
	}
}

void Keyboard::__arrow_left(PressedKey key) {
	if (key == Keyboard::PressedKey::ArrowLeft) {
		if ((Terminal::get_buffer_pos() > 1 && Keyboard::is_console_mode) || (Terminal::get_pos() && !Keyboard::is_console_mode)) {
			if (shift_pressed) {

				if (Terminal::get_highlighted_buffer_pos() == 0) Terminal::command.highlighted_buffer_start_pos = Terminal::get_buffer_pos();

				if (Terminal::get_highlighted_buffer_pos() <= 0) {
					Terminal::command.highlighted_buffer.push(Terminal::command.buffer[Terminal::get_buffer_pos() - 2], 0);
					Terminal::fill_bg_color_at(Terminal::get_pos() - 1, Terminal::Color::White);
					Terminal::set_highlighted_buffer_pos(Terminal::get_highlighted_buffer_pos() - 1);
				}

				else if (!Terminal::command.highlighted_buffer.empty()) {
					Terminal::command.highlighted_buffer.pop();
					Terminal::restore_bg_color_at(Terminal::get_pos() - 1);
					Terminal::set_highlighted_buffer_pos(Terminal::get_highlighted_buffer_pos() - 1);
				}
			}
			else {
				Terminal::clear_highlighted_buffer();
				Terminal::restore_default_bg_color();
			}

			Terminal::set_pos(Terminal::get_pos() - 1);
			Terminal::set_buffer_pos(Terminal::get_buffer_pos() > 1 ? Terminal::get_buffer_pos() - 1 : Terminal::get_buffer_pos());
			Terminal::move_cursor(Terminal::get_pos());
		}
	}
}

void Keyboard::__arrow_right(PressedKey key) {
	if (key == Keyboard::PressedKey::ArrowRight) {
		if (Terminal::get_buffer_pos() < Terminal::command.buffer.size() + 1) {
			if (shift_pressed) {

				if (Terminal::get_highlighted_buffer_pos() == 0) Terminal::command.highlighted_buffer_start_pos = Terminal::get_buffer_pos();

				if (Terminal::get_highlighted_buffer_pos() >= 0) {
					Terminal::command.highlighted_buffer.push(Terminal::command.buffer[Terminal::get_buffer_pos() - 1]);
					Terminal::fill_bg_color_at(Terminal::get_pos(), Terminal::Color::White);
					Terminal::set_highlighted_buffer_pos(Terminal::get_highlighted_buffer_pos() + 1);
				}

				else {
					Terminal::command.highlighted_buffer.pop(0);
					Terminal::restore_bg_color_at(Terminal::get_pos());
					Terminal::set_highlighted_buffer_pos(Terminal::get_highlighted_buffer_pos() + 1);
				}
			}
			else {
				Terminal::clear_highlighted_buffer();
				Terminal::restore_default_bg_color();
			}

			Terminal::set_pos(Terminal::get_pos() + 1);
			Terminal::set_buffer_pos(Terminal::get_buffer_pos() + 1);
			Terminal::move_cursor(Terminal::get_pos());
		}
	}
}