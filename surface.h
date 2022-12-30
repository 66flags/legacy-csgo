#pragma once

enum paint_modes_t {
	paint_uipanels = ( 1 << 0 ),
	paint_ingamepanels = ( 1 << 1 ),
};

enum font_flags_t {
	font_flag_none,
	font_flag_italic = 0x001,
	font_flag_underline = 0x002,
	font_flag_strike_out = 0x004,
	font_flag_symbol = 0x008,
	font_flag_anti_aliasing = 0x010,
	font_flag_gaussian_blur = 0x020,
	font_flag_rotary = 0x040,
	font_flag_drop_shadow = 0x080,
	font_flag_additive = 0x100,
	font_flag_outline = 0x200,
	font_flag_custom = 0x400,
	font_flag_bitmap = 0x800,
};

class c_engine_vgui {
public:
	PAD ( 48 );
	void *m_static_transition_panel;
};

class c_surface {
	enum indices_t : std::uintptr_t {
		_draw_set_color = 14,
		_draw_filled_rect = 16,
		_draw_outlined_rect = 18,
		_draw_line = 19,
		_draw_set_text_font = 23,
		_draw_set_text_color = 24,
		_draw_set_text_pos = 26,
		_draw_print_text = 28,
		_create_font = 71,
		_unlock_cursor = 66,
		_lock_cursor = 67,
		_get_text_size = 79,
		_set_font_glyph_set = 72,
	};
public:
	__forceinline void paint ( std::function< void ( ) > callback ) {
		start_drawing ( );
		callback ( );
		finish_drawing ( );
	}

	__forceinline void start_drawing ( ) {
		using start_drawing_fn = void ( __thiscall * )( void * );
		static auto o_start_drawing = pattern::find ( _ ( "vguimatsurface.dll" ), _ ( "55 8B EC 83 E4 C0 83 EC 38 80 3D ? ? ? ? ? 56 57 8B F9 75 53" ) ).as< start_drawing_fn > ( );
		o_start_drawing ( this );
	}

	__forceinline void finish_drawing ( ) {
		using finish_drawing_fn = void ( __thiscall * )( void * );
		static auto o_finish_drawing = pattern::find ( _ ( "vguimatsurface.dll" ), _ ( "8B 0D ? ? ? ? 56 C6 05 ? ? ? ? ? 8B 01 FF 90" ) ).as< finish_drawing_fn > ( );
		o_finish_drawing ( this );
	}

	void get_text_size ( unsigned long font_in, const wchar_t *text, int &wide, int &tall ) {
		using get_text_size_fn = void ( __thiscall * )( void *, unsigned long, const wchar_t *, int &, int & );
		util::get_method< get_text_size_fn > ( this, indices_t::_get_text_size )( this, font_in, text, wide, tall );
	}

	bool set_font_glyph_set ( unsigned long font_in, const char *font_name, int tall, int weight, int blur, int scan_lines, int flags, int range_min = 0, int range_max = 0 ) {
		using set_font_glyph_set_fn = bool ( __thiscall * )( void *, unsigned long, const char *, int, int, int, int, int, int, int );
		return util::get_method< set_font_glyph_set_fn > ( this, indices_t::_set_font_glyph_set )( this, font_in, font_name, tall, weight, blur, scan_lines, flags, range_min, range_max );
	}

	void draw_set_color ( color_t color ) {
		using draw_set_color_fn = void ( __thiscall * )( void *, color_t );
		util::get_method< draw_set_color_fn > ( this, indices_t::_draw_set_color )( this, color );
	}

	void draw_filled_rect ( int x0, int y0, int x1, int y1 ) {
		using draw_filled_rect_fn = void ( __thiscall * )( void *, int, int, int, int );
		util::get_method< draw_filled_rect_fn > ( this, indices_t::_draw_filled_rect )( this, x0, y0, x1, y1 );
	}

	void draw_outlined_rect ( int x0, int y0, int x1, int y1 ) {
		using draw_outlined_rect_fn = void ( __thiscall * )( void *, int, int, int, int );
		util::get_method< draw_outlined_rect_fn > ( this, indices_t::_draw_outlined_rect )( this, x0, y0, x1, y1 );
	}

	void draw_line ( int x0, int y0, int x1, int y1 ) {
		using draw_line_fn = void ( __thiscall * )( void *, int, int, int, int );
		util::get_method< draw_line_fn > ( this, indices_t::_draw_line )( this, x0, y0, x1, y1 );
	}

	void draw_set_text_font ( unsigned long font_in ) {
		using draw_set_text_font_fn = void ( __thiscall * )( void *, unsigned long );
		util::get_method< draw_set_text_font_fn > ( this, indices_t::_draw_set_text_font )( this, font_in );
	}

	void draw_set_text_color ( color_t color ) {
		using draw_set_text_color_fn = void ( __thiscall * )( void *, color_t );
		util::get_method< draw_set_text_color_fn > ( this, indices_t::_draw_set_text_color )( this, color );
	}

	void draw_set_text_pos ( int x, int y ) {
		using draw_set_text_pos_fn = void ( __thiscall * )( void *, int, int );
		util::get_method< draw_set_text_pos_fn > ( this, indices_t::_draw_set_text_pos )( this, x, y );
	}

	void draw_print_text ( const wchar_t *text, int len, int draw_fn = 0 ) {
		using draw_print_text_fn = void ( __thiscall * )( void *, const wchar_t *, int, int );
		util::get_method< draw_print_text_fn > ( this, indices_t::_draw_print_text )( this, text, len, draw_fn );
	}

	unsigned long create_font ( ) {
		using create_font_fn = unsigned long ( __thiscall * )( void * );
		return util::get_method< create_font_fn > ( this, indices_t::_create_font )( this );
	}

	void unlock_cursor ( ) {
		using unlock_cursor_fn = void ( __thiscall * )( void * );
		util::get_method< unlock_cursor_fn > ( this, indices_t::_unlock_cursor )( this );
	}

	void lock_cursor ( ) {
		using lock_cursor_fn = void ( __thiscall * )( void * );
		util::get_method< lock_cursor_fn > ( this, indices_t::_lock_cursor )( this );
	}
};