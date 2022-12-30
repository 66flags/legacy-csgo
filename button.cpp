#include "includes.h"

void ui::button::paint ( ) {
	auto dim = draw_list::menu.size ( this->m_name );

	draw_list::gradient ( this->m_area.x, this->m_area.y, this->m_area.w, this->m_area.h, !this->blocked ( ) && this->m_held ? color_t { 18, 18, 18 } : color_t { 29, 29, 29 }, !this->blocked ( ) && this->m_held ? color_t { 29, 29, 29 } : color_t { 18, 18, 18 }, true, true );
	draw_list::rect_outlined ( this->m_area.x, this->m_area.y, this->m_area.w, this->m_area.h, { 0, 0, 0 } );
	draw_list::menu.string ( this->m_area.x + this->m_area.w / 2 - dim.m_width / 2, this->m_area.y + this->m_area.h / 2 - dim.m_height / 2, { 200, 200, 200 }, m_name, draw_list::ALIGN_LEFT, false, true );
}

void ui::button::think ( ) {
	auto dim = draw_list::menu.size ( this->m_name );

	if ( !this->blocked ( ) ) {
		this->m_hovered = inputsys.hovered ( { this->m_area.x, this->m_area.y, this->m_area.w + dim.m_width + 6.f, this->m_area.h } );

		if ( this->m_hovered && inputsys.key_pressed ( VK_LBUTTON ) )
			this->m_click = true;
	}

	/* just make it not click whenever we click before then hover over our object */ {
		if ( this->m_click ) {
			m_click = false;
			m_held = true;
			this->m_callback ( );
		}

		if ( m_held && !inputsys.key_down ( VK_LBUTTON ) )
			m_held = false;
	}
}