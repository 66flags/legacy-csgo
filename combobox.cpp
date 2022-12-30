#include "includes.h"

void ui::combobox::paint ( ) {
	draw_list::menu.string ( this->m_area.x + 1.5f, this->m_area.y, { 200, 200, 200 }, m_name, draw_list::ALIGN_LEFT, false, true );
	draw_list::gradient ( this->m_area.x, this->m_area.y + m_offset, this->m_area.w, this->m_area.h, { 29, 29, 29 }, { 18, 18, 18 }, true );
	draw_list::rect_outlined ( this->m_area.x, this->m_area.y + m_offset, this->m_area.w, this->m_area.h, { 0, 0, 0 } );
	draw_list::menu.string ( this->m_area.x + 5.f, this->m_area.y + m_offset + 2.0f, { 200, 200, 200 }, this->m_items [ *this->m_value ], draw_list::ALIGN_LEFT, false, true );

	if ( focused != this )
		return;

	draw_list::rect_filled ( m_dropdown_area.x, m_dropdown_area.y, m_dropdown_area.w, m_dropdown_area.h, { 18, 18, 18 } );

	for ( int i = 0; i < m_items.size ( ); i++ ) {
		draw_list::rect_filled ( m_dropdown_area.x, m_dropdown_area.y + ( i * this->m_area.h ), m_dropdown_area.w, this->m_area.h, { 18, 18, 18 } );
		draw_list::menu.string ( m_dropdown_area.x + 5.f, m_dropdown_area.y + ( i * this->m_area.h ) + 2.0f, i == *m_value ? color_t { 91, 164, 255 } : color_t { 150, 150, 150 }, m_items [ i ], draw_list::ALIGN_LEFT, false, true );
	}

	draw_list::rect_outlined ( m_dropdown_area.x, m_dropdown_area.y, m_dropdown_area.w, m_dropdown_area.h, { 0, 0, 0 } );
}

void ui::combobox::think ( ) {
	auto dim = draw_list::menu.size ( m_name );

	if ( !this->blocked ( ) ) {
		this->m_hovered = inputsys.hovered ( { this->m_area.x, this->m_area.y + m_offset, this->m_area.w, this->m_area.h } );

		if ( this->m_hovered && inputsys.key_pressed ( VK_LBUTTON ) )
			focused = this;
	}

	if ( focused != this )
		return;

	this->m_focus_area = { this->m_area.x, ( this->m_area.y + m_offset ) + this->m_area.h - 1.0f, this->m_area.w, this->m_area.h };

	for ( int i = 0; i < m_items.size ( ); i++ ) {
		if ( inputsys.hovered ( { m_dropdown_area.x, m_dropdown_area.y + ( i * this->m_area.h ), m_dropdown_area.w, this->m_area.h } ) && inputsys.key_pressed ( VK_LBUTTON ) ) {
			*this->m_value = i;
			break;
		}
	}

	this->m_dropdown_area = { this->m_focus_area.x, this->m_focus_area.y, this->m_focus_area.w, this->m_items.size ( ) * this->m_area.h };

	if ( ( !inputsys.hovered ( this->m_dropdown_area ) && !inputsys.hovered ( { this->m_area.x, this->m_area.y + m_offset, this->m_area.w, this->m_area.h } ) ) && inputsys.key_pressed ( VK_LBUTTON ) )
		focused = nullptr;
}