#include "includes.h"

void ui::checkbox::paint ( ) {
	draw_list::gradient ( this->m_area.x, this->m_area.y, this->m_area.w, this->m_area.h, { 29, 29, 29 }, { 18, 18, 18 }, true );

	if ( *this->m_value )
		draw_list::gradient ( this->m_area.x, this->m_area.y, this->m_area.w, this->m_area.h, { 91, 164, 255 }, { 82, 148, 230 }, true );

	draw_list::rect_outlined ( this->m_area.x, this->m_area.y, this->m_area.w, this->m_area.h, { 0, 0, 0 } );
	draw_list::menu.string ( this->m_area.x + this->m_area.w + 5.0f, this->m_area.y, { 200, 200, 200 }, m_name, draw_list::ALIGN_LEFT, false, true );
}

void ui::checkbox::think ( ) {
	auto dim = draw_list::menu.size ( this->m_name );

	if ( !this->blocked ( ) ) {
		this->m_hovered = inputsys.hovered ( { this->m_area.x, this->m_area.y, this->m_area.w + dim.m_width + 6.f, this->m_area.h } );

		if ( this->m_hovered && inputsys.key_pressed ( VK_LBUTTON ) )
			*this->m_value = !*this->m_value;
	}
}