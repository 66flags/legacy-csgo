#pragma once

enum class fonts_t {
	default_font,
	log_font,
	debug_font
};

struct font_data_t {
	const char *m_name;
	int m_height;
	int m_weight;
	int m_blur;
	int m_flags;
	unsigned long m_data;

	__forceinline font_data_t ( ) = default;
	__forceinline font_data_t ( const char *name, int height, int weight, int blur, int flags ) : m_name ( name ), m_height ( height ), m_weight ( weight ), m_blur ( blur ), m_flags ( flags ), m_data ( 0 ) { }
};

inline std::map < fonts_t, font_data_t > fonts;

struct render_t {
	void init ( );
	void filled_rect ( int x, int y, int w, int h, color_t color );
	void outlined_rect ( int x, int y, int w, int h, color_t color );
	void line ( int x0, int y0, int x1, int y1, color_t color );
	void string ( unsigned long font_in, int x, int y, color_t color, const char *format, ... );
	void string ( unsigned long font_in, int x, int y, color_t color, wchar_t *data );
	bool world_to_screen ( const vec_t &origin, vec_t &screen );
	vec2_t get_text_size ( const unsigned long &font, const char *fmt, ... );
	vec2_t get_text_size ( const unsigned long &font, wchar_t *data );

	int m_width, m_height;
};

extern render_t render;