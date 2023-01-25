#include "includes.h"

render_t render { };

void render_t::init ( ) {
	unsigned long font_count = 0;
	LI_FN ( AddFontMemResourceEx ) ( hooge_ttf, sizeof ( hooge_ttf ), nullptr, &font_count );

	/* update screen size. */
	interfaces::m_engine->get_screen_size ( m_width, m_height );

	/* create font data. */
	fonts [ fonts_t::font_esp_name ] = font_data_t ( _ ( "Verdana" ), 12, FW_BOLD, 0, font_flags_t::font_flag_drop_shadow );
	fonts [ fonts_t::font_esp_small ] = font_data_t ( _ ( "hooge 05_55 Cyr2" ), 9, 400, 0, font_flags_t::font_flag_outline );

	/* iterate and create fonts. */
	for ( auto &font : fonts ) {
		auto &font_data = font.second;

		font_data.m_data = interfaces::m_surface->create_font ( );

		if ( font_data.m_data != 0 )
			interfaces::m_surface->set_font_glyph_set ( font_data.m_data, font_data.m_name, font_data.m_height, font_data.m_weight, font_data.m_blur, 0, font_data.m_flags );
	}
}

void render_t::filled_rect ( int x, int y, int w, int h, color_t color ) {
	interfaces::m_surface->draw_set_color ( color );
	interfaces::m_surface->draw_filled_rect ( x, y, x + w, y + h );
}

void render_t::outlined_rect ( int x, int y, int w, int h, color_t color ) {
	interfaces::m_surface->draw_set_color ( color );
	interfaces::m_surface->draw_outlined_rect ( x, y, x + w, y + h );
}

void render_t::line ( int x0, int y0, int x1, int y1, color_t color ) {
	interfaces::m_surface->draw_set_color ( color );
	interfaces::m_surface->draw_line ( x0, y0, x1, y1 );
}

void render_t::string ( unsigned long font_in, int x, int y, color_t color, const char *format, ... ) {
	va_list va_alist;
	char buffer [ 1024 ] = { 0 };

	va_start ( va_alist, format );
	vsprintf_s ( buffer, format, va_alist );
	va_end ( va_alist );

	wchar_t wbuffer [ 1024 ] = { 0 };
	MultiByteToWideChar ( CP_UTF8, 0, buffer, -1, wbuffer, sizeof ( wbuffer ) );

	interfaces::m_surface->draw_set_text_font ( font_in );
	interfaces::m_surface->draw_set_text_color ( color );
	interfaces::m_surface->draw_set_text_pos ( x, y );
	interfaces::m_surface->draw_print_text ( wbuffer, wcslen ( wbuffer ) );
}

void render_t::string ( unsigned long font_in, int x, int y, color_t color, wchar_t *data ) {
	interfaces::m_surface->draw_set_text_font ( font_in );
	interfaces::m_surface->draw_set_text_color ( color );
	interfaces::m_surface->draw_set_text_pos ( x, y );
	interfaces::m_surface->draw_print_text ( data, wcslen ( data ) );
}

bool render_t::world_to_screen ( const vec_t &origin, vec_t &screen ) {
	/* https://github.com/perilouswithadollarsign/cstrike15_src/blob/master/engine/gl_rmain.cpp#L849 */
	static auto view_matrix = pattern::find ( _ ( "client.dll" ), _ ( "0F 10 05 ? ? ? ? 8D 85 ? ? ? ? B9" ) ).add ( 3 ).deref ( ).add ( 176 ).as< std::uintptr_t > ( );
	const auto &world_matrix = *reinterpret_cast< matrix3x4_t * >( view_matrix );

	screen.x = world_matrix [ 0 ][ 0 ] * origin.x + world_matrix [ 0 ][ 1 ] * origin.y + world_matrix [ 0 ][ 2 ] * origin.z + world_matrix [ 0 ][ 3 ];
	screen.y = world_matrix [ 1 ][ 0 ] * origin.x + world_matrix [ 1 ][ 1 ] * origin.y + world_matrix [ 1 ][ 2 ] * origin.z + world_matrix [ 1 ][ 3 ];
	float w = world_matrix [ 3 ][ 0 ] * origin.x + world_matrix [ 3 ][ 1 ] * origin.y + world_matrix [ 3 ][ 2 ] * origin.z + world_matrix [ 3 ][ 3 ];

	if ( w < 0.001f ) {
		screen.x *= 100000.0f;
		screen.y *= 100000.0f;
		return false;
	}

	screen.x *= 1.0f / w;
	screen.y *= 1.0f / w;

	screen.x = ( m_width / 2 ) + ( screen.x * m_width ) / 2;
	screen.y = ( m_height / 2 ) - ( screen.y * m_height ) / 2;

	return true;
}

vec2_t render_t::get_text_size ( const unsigned long &font, const char *fmt, ... ) {
	va_list va_alist;
	char buffer [ 1024 ] = { 0 };

	va_start ( va_alist, fmt );
	vsprintf_s ( buffer, fmt, va_alist );
	va_end ( va_alist );

	wchar_t wbuffer [ 1024 ] = { 0 };
	MultiByteToWideChar ( CP_UTF8, 0, buffer, -1, wbuffer, sizeof ( wbuffer ) );

	int width, height;
	interfaces::m_surface->get_text_size ( font, wbuffer, width, height );

	return { static_cast< float >( width ), static_cast< float >( height ) };
}

vec2_t render_t::get_text_size ( const unsigned long &font, wchar_t *data ) {
	int width, height;
	interfaces::m_surface->get_text_size ( font, data, width, height );

	return { static_cast< float >( width ), static_cast< float >( height ) };
}