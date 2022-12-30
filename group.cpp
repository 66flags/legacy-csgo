#include "includes.h"

void ui::group::paint ( ) {
	draw_list::rect_outlined ( this->m_area.x - 1, this->m_area.y - 1, this->m_area.w + 2, this->m_area.h + 2, { 17, 17, 17 } );
	draw_list::rect_filled ( this->m_area.x, this->m_area.y, this->m_area.w, this->m_area.h, { 24, 24, 24 } );
	draw_list::gradient ( this->m_area.x, this->m_area.y, this->m_area.w, 18, { 20, 20, 20 }, { 9, 9, 9 }, true );
	draw_list::rect_outlined ( this->m_area.x, this->m_area.y, this->m_area.w, 18, { 12, 12, 12 } );
	draw_list::menu.string ( this->m_area.x + 7, this->m_area.y + 4.0f, { 255, 255, 255 }, m_name, draw_list::ALIGN_LEFT, true, false );

	draw_list::clip ( this->m_area.x, this->m_area.y + 18.f, this->m_area.w, this->m_area.h - 18.f, [ & ] {
		for ( auto &object : objects ) {
			if ( focused && focused == object.get ( ) )
				continue;

			object->paint ( );
		}
		} );

	draw_list::gradient ( this->m_area.x, this->m_area.y + this->m_area.h - 10.f, this->m_area.w, 10.f, { 24, 24, 24, 0 }, { 24, 24, 24 }, true );
	draw_list::gradient ( this->m_area.x, this->m_area.y + 16.f, this->m_area.w, 10.f, { 24, 24, 24 }, { 24, 24, 24, 0 }, true );
	draw_list::rect_outlined ( this->m_area.x, this->m_area.y, this->m_area.w, this->m_area.h, { 0, 0, 0 } );
}

void ui::group::think ( ) {
	this->m_hovered = inputsys.hovered ( m_area );
	this->m_offset = { 0.0f, 0.0f };

	base_control *last_control { };

	for ( auto &object : objects ) {
		object->m_area = {
			this->m_area.x + 10.f,
			this->m_area.y + 23.f + this->m_offset.y - this->m_scroll_offset,
			0.f,
			0.f
		};

		object->m_parent = this;

		switch ( object->m_type ) {
		case control_type_t::CONTROL_CHECKBOX: {
			object->m_area.w = 9.f;
			object->m_area.h = 9.f;
			object->m_padding = 5.f;

			this->m_offset.y += object->m_area.h + object->m_padding;
		} break;
		case control_type_t::CONTROL_TEXTBOX: {
			object->m_area.w = 180.f;
			object->m_area.h = 14.f;
			object->m_padding = object->m_offset + 5.f;

			this->m_offset.y += object->m_area.h + object->m_padding;
		} break;
		case control_type_t::CONTROL_MULTICOMBOBOX: {
			object->m_area.w = 180.f;
			object->m_area.h = 14.f;
			object->m_padding = object->m_offset + 5.f;

			this->m_offset.y += object->m_area.h + object->m_padding;
		} break;
		case control_type_t::CONTROL_SLIDER: {
			object->m_area.w = 180.f;
			object->m_area.h = 7.f;
			object->m_padding = object->m_offset + 5.f;

			this->m_offset.y += object->m_area.h + object->m_padding;
		} break;
		case control_type_t::CONTROL_COMBOBOX: {
			object->m_area.w = 180.f;
			object->m_area.h = 14.f;
			object->m_padding = object->m_offset + 5.f;

			this->m_offset.y += object->m_area.h + object->m_padding;
		} break;
		case control_type_t::CONTROL_COLORPICKER: {
			object->m_area.w = 17.f;
			object->m_area.h = 8.f;
		} break;
		case control_type_t::CONTROL_KEYBIND: {
			object->m_area.w = 17.f;
			object->m_area.h = 8.f;
		} break;
		case control_type_t::CONTROL_BUTTON: {
			object->m_area.w = 180.f;
			object->m_area.h = 25.f;
			object->m_padding = object->m_offset + 5.f;

			this->m_offset.y += object->m_area.h + object->m_padding;
		} break;
		}

		if ( object->m_padding != 0 ) {
			if ( last_control == nullptr ) {
				m_first_pad = object->m_padding - object->m_offset;

				last_control = object.get ( );
			}
		}

		m_last_pad = object->m_padding - object->m_offset;

		if ( focused && focused != object.get ( ) )
			continue;

		object->think ( );
	}

	if ( this->m_hovered ) {
		m_should_scroll = focused == nullptr;

		if ( !m_should_scroll )
			return;

		int max_height = this->m_area.h > this->m_offset.y + ( this->m_first_pad + this->m_last_pad ) + 23.f ? this->m_area.h : this->m_offset.y + ( this->m_first_pad + this->m_last_pad ) + 23.f;

		if ( this->m_hovered && scroll_delta != 0.0 ) {
			this->m_scroll_offset += scroll_delta * 15.f;
			scroll_delta = 0.0;
		}

		this->m_scroll_offset = std::clamp< float > ( this->m_scroll_offset, 0, max_height - this->m_area.h );
	}
}