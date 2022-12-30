#include "includes.h"

std::string keys_list [ 253 ] = {
   "NONE", "LEFT MOUSE", "RIGHT MOUSE", "BREAK", "MOUSE 3", "MOUSE 4", "MOUSE 5",
   "ERROR", "BCKSPCE", "TAB", "ERROR", "ERROR", "ERROR", "ENTER", "ERROR", "ERROR", "SHIFT",
   "CONTROL", "ALT", "PAUSE", "CAPS LOCK", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
   "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "SPACEBAR", "PG UP", "PG DOWN", "END", "HOME", "LEFT",
   "UP", "RIGHT", "DOWN", "ERROR", "PRINT", "ERROR", "PRINT SCREEN", "INSERT", "DELETE", "ERROR", "0", "1",
   "2", "3", "4", "5", "6", "7", "8", "9", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
   "ERROR", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U",
   "V", "W", "X", "Y", "Z", "LWIN", "RIGHT WINDOWS", "ERROR", "ERROR", "ERROR", "NUM 0", "NUM 1",
   "NUM 2", "NUM 3", "NUM 4", "NUM 5", "NUM 6", "NUM 7", "NUM 8", "NUM 9", "*", "+", "_", "-", ".", "/", "F1", "F2", "F3",
   "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "F13", "F14", "F15", "F16", "F17", "F18", "F19", "F20", "F21",
   "F22", "F23", "F24", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
   "NUM LOCK", "SCROLL LOCK", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
   "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "LSHIFT", "RSHIFT", "LCONTROL",
   "RCONTROL", "LMENU", "RMENU", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
   "ERROR", "ERROR", "ERROR", "NEXT TRACK", "PREVIOUS TRACK", "STOP", "PLAY/PAUSE", "ERROR", "ERROR",
   "ERROR", "ERROR", "ERROR", "ERROR", ";", "+", ",", "-", ".", "/?", "~", "ERROR", "ERROR",
   "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
   "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
   "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "[{", "\\|", "}]", "'\"", "ERROR",
   "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
   "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
   "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR", "ERROR",
   "ERROR", "ERROR"
};

void ui::keybind::paint ( ) {
	auto dim = draw_list::menu_small.size ( this->m_set ? "WAITING..." : keys_list [ *this->m_key ] );

	draw_list::gradient ( this->m_item_area.x, this->m_item_area.y, this->m_item_area.w, this->m_item_area.h, { 29, 29, 29 }, { 18, 18, 18 }, true );;
	draw_list::rect_outlined ( this->m_item_area.x, this->m_item_area.y, this->m_item_area.w, this->m_item_area.h, { 0, 0, 0 } );
	draw_list::menu_small.string ( this->m_item_area.x + ( this->m_item_area.w / 2 ) + 0.5f, this->m_item_area.y + ( this->m_item_area.h / 2 - dim.m_height / 2 ) + 1.0f, this->m_set ? color_t { 91, 164, 255 } : color_t { 200, 200, 200 }, this->m_set ? "WAITING..." : keys_list [ *this->m_key ], draw_list::ALIGN_CENTER, true, false );

	if ( focused != this )
		return;

	draw_list::rect_filled ( this->m_box_rect.x, this->m_box_rect.y, this->m_box_rect.w, this->m_box_rect.h, { 18, 18, 18 } );

	for ( int i = 0; i < this->m_key_names.size ( ); i++ ) {
		auto item_dim = draw_list::menu.size ( this->m_key_names [ i ] );

		bool hovered = inputsys.hovered ( { this->m_box_rect.x, this->m_box_rect.y + ( i * 15.f ), m_box_rect.w, 15.f } );

		draw_list::rect_filled ( this->m_box_rect.x, this->m_box_rect.y + ( i * 15.f ), m_box_rect.w, 15.f, { 18, 18, 18 } );
		draw_list::menu.string ( this->m_box_rect.x + this->m_box_rect.w - item_dim.m_width - 3.f, this->m_box_rect.y + ( i * 15.f ) + 2.5f, ( i == *this->m_key_type || hovered ) ? color_t { 91, 164, 255 } : color_t { 150, 150, 150 }, this->m_key_names [ i ], draw_list::ALIGN_LEFT, false, true );
	}

	draw_list::rect_outlined ( this->m_box_rect.x, this->m_box_rect.y, this->m_box_rect.w, this->m_box_rect.h, { 0, 0, 0, 255 } );
}

void ui::keybind::think ( ) {
	if ( focused != this ) {
		if ( ( !this->m_set && !this->m_old_set ) && ( this->m_hovered && inputsys.key_pressed ( VK_LBUTTON ) ) )
			this->m_set = true;

		else if ( this->m_set ) {
			for ( int i = 0; i < 253; i++ ) {
				if ( inputsys.key_pressed ( i ) ) {
					*this->m_key = i;

					this->m_set = false;
					this->m_old_set = true;

					return;
				}
			}
		}

		else this->m_old_set = false;

		if ( ( !this->m_set && this->m_hovered ) && inputsys.key_pressed ( VK_RBUTTON ) )
			focused = this;
	}

	auto dim = draw_list::menu_small.size ( this->m_set ? "WAITING..." : keys_list [ *this->m_key ] );

	this->m_item_area = { this->m_area.x + 180.f - dim.m_width - 9.f, this->m_area.y, dim.m_width + 9.f, this->m_area.h + 3.f };

	if ( !this->blocked ( ) ) {
		this->m_hovered = inputsys.hovered ( this->m_item_area );
	}

	this->m_box_rect = { this->m_item_area.x + this->m_item_area.w + 3.f, this->m_item_area.y, 70.f, this->m_key_names.size ( ) * 15.f };

	if ( focused != this )
		return;

	for ( int i = 0; i < this->m_key_names.size ( ); i++ ) {
		if ( inputsys.hovered ( { this->m_box_rect.x, this->m_box_rect.y + ( i * 15.f ), m_box_rect.w, 15.f } ) && inputsys.key_pressed ( VK_LBUTTON ) ) {
			*this->m_key_type = i;
			focused = nullptr;
			break;
		}
	}

	if ( !inputsys.hovered ( this->m_box_rect ) && !inputsys.hovered ( this->m_item_area ) && inputsys.key_pressed ( VK_LBUTTON ) )
		focused = nullptr;
}