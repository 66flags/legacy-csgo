#include "includes.h"

void ui::tab::paint ( ) {
	draw_list::gradient ( this->m_area.x, this->m_area.y, this->m_area.w, this->m_area.h, this->m_visible ? color_t { 29, 29, 29 } : color_t { 18, 18, 18 }, this->m_visible ? color_t { 20, 20, 20 } : color_t { 9, 9, 9 }, true );
	draw_list::menu.string ( this->m_area.x + this->m_area.w / 2, this->m_area.y + 5.f, this->m_visible ? color_t { 91, 164, 255 } : color_t { 226, 255, 255 }, m_name, draw_list::ALIGN_CENTER, true, false );
	draw_list::rect_outlined ( this->m_area.x, this->m_area.y, this->m_area.w, this->m_area.h, { 0, 0, 0 } );

	if ( !this->m_visible )
		return;

	for ( auto &child : this->childs )
		child->paint ( );
}

void ui::tab::think ( ) {
	if ( !this->m_visible )
		return;

	if ( !childs.empty ( ) ) {
		for ( int i { }; i < childs.size ( ); i++ ) {
			auto &child = childs [ i ];

			child->m_area = {
				m_parent->m_area.x + child->m_draw_area.x + 11.f,
				m_parent->m_area.y + child->m_draw_area.y + 45.f,
				child->m_draw_area.w,
				child->m_draw_area.h
			};

			child->think ( );
		}
	}
}