#include "includes.h"

/* credits: zxvnme */
ui::key_code_info_t special_characters [ 22 ] = {
	{ 48,  '0',  ')' },
	{ 49,  '1',  '!' },
	{ 50,  '2',  '@' },
	{ 51,  '3',  '#' },
	{ 52,  '4',  '$' },
	{ 53,  '5',  '%' },
	{ 54,  '6',  '^' },
	{ 55,  '7',  '&' },
	{ 56,  '8',  '*' },
	{ 57,  '9',  '(' },
	{ 32,  ' ',  ' ' },
	{ 192, '`',  '~' },
	{ 189, '-',  '_' },
	{ 187, '=',  '+' },
	{ 219, '[',  '{' },
	{ 220, '\\', '|' },
	{ 221, ']',  '}' },
	{ 186, ';',  ':' },
	{ 222, '\'', '"' },
	{ 188, ',',  '<' },
	{ 190, '.',  '>' },
	{ 191, '/',  '?' }
};

void ui::textbox::paint ( ) {
	draw_list::menu.string ( this->m_area.x + 1.5f, this->m_area.y, { 200, 200, 200 }, m_name, draw_list::ALIGN_LEFT, false, true );
	draw_list::gradient ( this->m_area.x, this->m_area.y + m_offset, this->m_area.w, this->m_area.h, { 29, 29, 29 }, { 18, 18, 18 }, true );
	draw_list::rect_outlined ( this->m_area.x, this->m_area.y + m_offset, this->m_area.w, this->m_area.h, { 0, 0, 0 } );
	draw_list::menu.string ( this->m_area.x + 5.f, this->m_area.y + m_offset + 2.0f, { 200, 200, 200 }, *this->m_data, draw_list::ALIGN_LEFT, false, true );

	auto dim = draw_list::menu.size ( *this->m_data );

	if ( this->m_set )
		draw_list::menu.string ( this->m_area.x + dim.m_width + 5.f, this->m_area.y + m_offset + 2.0f, { 255, 255, 255, 255 }, "_", draw_list::ALIGN_LEFT, false, true );
}

void ui::textbox::think ( ) {
	auto dim = draw_list::menu.size ( m_name );

	if ( !this->blocked ( ) ) {
		this->m_hovered = inputsys.hovered ( { this->m_area.x, this->m_area.y + m_offset, this->m_area.w, this->m_area.h } );

		if ( focused != this ) {
			if ( this->m_hovered && inputsys.key_pressed ( VK_LBUTTON ) )
				this->m_set = true;
		}
	}

	if ( this->m_set ) {
		if ( ( !this->m_hovered && inputsys.key_pressed ( VK_LBUTTON ) )
			|| inputsys.key_pressed ( VK_RETURN ) || inputsys.key_pressed ( VK_ESCAPE ) )
			this->m_set = false;

		if ( inputsys.key_pressed ( VK_BACK ) ) {
			if ( m_data->size ( ) > 0 )
				m_data->pop_back ( );
		}

		if ( m_data->length ( ) < this->m_size ) {
			for ( int i = 32; i <= 222; i++ ) {
				if ( ( i > 32 && i < 48 ) || ( i > 57 && i < 65 ) || ( i > 90 && i < 186 ) )
					continue;

				if ( i > 57 && i <= 90 ) {
					if ( inputsys.key_pressed ( i ) )
						*this->m_data += inputsys.key_down ( VK_SHIFT ) ? static_cast< char >( i ) : static_cast< char >( i + 32 );
				}
				else {
					if ( inputsys.key_pressed ( i ) ) {
						for ( int j = 0; j < sizeof ( special_characters ); j++ ) {
							if ( special_characters [ j ].vk == i )
								*this->m_data += inputsys.key_down ( VK_SHIFT ) ? special_characters [ j ].shift : special_characters [ j ].regular;
						}
					}
				}
			}
		}
	}
}