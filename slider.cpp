#include "includes.h"

void ui::slider::paint ( ) {
	draw_list::menu.string ( this->m_area.x + 1.5f, this->m_area.y, { 200, 200, 200 }, m_name, draw_list::ALIGN_LEFT, false, true );
	draw_list::gradient ( this->m_area.x, this->m_area.y + this->m_offset, this->m_area.w, this->m_area.h, { 29, 29, 29 }, { 18, 18, 18 }, true );
	draw_list::gradient ( this->m_area.x, this->m_area.y + this->m_offset, this->m_fill, this->m_area.h, { 91, 164, 255 }, { 82, 148, 230 }, true );
	draw_list::rect_outlined ( this->m_area.x, this->m_area.y + this->m_offset, this->m_area.w, this->m_area.h, { 0, 0, 0 } );

	auto percentage = std::to_string ( int ( *this->m_value ) );
	auto dim = draw_list::menu.size ( percentage );

	draw_list::menu.string ( this->m_area.x + this->m_area.w - dim.m_width, this->m_area.y, { 200, 200, 200 }, percentage, draw_list::ALIGN_LEFT, false, true );
}

void ui::slider::think ( ) {
	this->m_fill = this->m_area.w * std::clamp< float > ( ( *this->m_value - this->m_min ) / ( this->m_max - this->m_min ), 0.f, 1.f );

	if ( !this->blocked ( ) ) {
		this->m_hovered = inputsys.hovered ( { this->m_area.x, this->m_area.y + this->m_offset, this->m_area.w, this->m_area.h } );

		if ( this->m_hovered && inputsys.key_pressed ( VK_LBUTTON ) )
			this->m_drag = true;
	}

	if ( this->m_drag && inputsys.key_down ( VK_LBUTTON ) )
		*this->m_value = std::clamp< float > ( ( inputsys.m_pos.x - this->m_area.x ) / this->m_area.w, 0.f, 1.f ) * ( this->m_max - this->m_min ) + this->m_min;

	if ( this->m_drag && !inputsys.key_down ( VK_LBUTTON ) )
		this->m_drag = false;
}