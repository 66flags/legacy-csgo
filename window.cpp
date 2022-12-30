#include "includes.h"

void ui::window::paint ( ) {
	draw_list::clip ( this->m_area.x, this->m_area.y, this->m_area.w - 1.0f, this->m_area.h, [ & ] {
		draw_list::rect_filled ( this->m_area.x, this->m_area.y, this->m_area.w, this->m_area.h, { 14, 14, 14 } );
		draw_list::gradient ( this->m_area.x, this->m_area.y, this->m_area.w, 16, { 20, 20, 20 }, { 9, 9, 9 }, true );
		draw_list::menu.string ( this->m_area.x + 6, this->m_area.y + 4, { 255, 255, 255 }, this->m_name, draw_list::ALIGN_LEFT, true );

		for ( auto &tab : tabs )
			tab->paint ( );
	} );

	draw_list::rect_outlined ( this->m_area.x, this->m_area.y, this->m_area.w, this->m_area.h, { 0, 0, 0 } );

	if ( focused )
		focused->paint ( );
}

void ui::window::think ( ) {
	this->m_hovered = inputsys.hovered ( { m_area.x, m_area.y, m_area.w, 16.f } );

	if ( !m_drag && ( this->m_hovered && inputsys.key_pressed ( VK_LBUTTON ) ) ) {
		this->m_delta.x = inputsys.m_pos.x - this->m_area.x;
		this->m_delta.y = inputsys.m_pos.y - this->m_area.y;
		this->m_drag = true;
	}

	if ( m_drag ) {
		this->m_area.x = inputsys.m_pos.x - this->m_delta.x;
		this->m_area.y = inputsys.m_pos.y - this->m_delta.y;
	}

	if ( m_drag && !inputsys.key_down ( VK_LBUTTON ) )
		m_drag = false;

	if ( !tabs.empty ( ) ) {
		for ( int i { }; i < tabs.size ( ); i++ ) {
			auto &tab = tabs [ i ];

			tab->m_parent = this;

			tab->m_area = {
				this->m_area.x + i * ( this->m_area.w / tabs.size ( ) ) ,
				this->m_area.y + 16.0f,
				( this->m_area.w / tabs.size ( ) ) + 1.0f,
				18.0f
			};

			tab->m_hovered = inputsys.hovered ( tab->m_area );

			if ( tab->m_hovered && inputsys.key_pressed ( VK_LBUTTON ) )
				this->m_page = i;

			tab->m_visible = this->m_page == i;

			tab->think ( );
		}
	}
}