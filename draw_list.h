#pragma once

namespace draw_list {
	struct font_dim_t {
		float m_width;
		float m_height;
	};

	enum string_flags_t {
		ALIGN_LEFT = 0,
		ALIGN_RIGHT,
		ALIGN_CENTER
	};

	enum font_flag_t : int {
		NONE = 0,
		MONOHINTING = 1,
		MONOCHROME = 2
	};

	__forceinline void invalidate ( ) {
		ImGui_ImplDX9_InvalidateDeviceObjects ( );
		ImGui_ImplDX9_CreateDeviceObjects ( );
	}

	class c_font {
	public:
		ImFont *m_handle;
		font_dim_t m_size;
	public:
		__forceinline c_font ( ) : m_handle {}, m_size {} {};

		// ctor.
		__forceinline c_font ( const ImGuiIO &io, const std::string &name, const float size, const ImWchar *ranges = 0, const int rasterizer_flags = 0 ) {
			ImFontConfig cfg;

			if ( rasterizer_flags & font_flag_t::MONOHINTING )
				cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_MonoHinting;

			if ( rasterizer_flags & font_flag_t::MONOCHROME )
				cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Monochrome;

			m_handle = io.Fonts->AddFontFromFileTTF ( name.data ( ), size, &cfg, ranges );
			m_size.m_height = size;
		}

		// ctor.
		__forceinline c_font ( const ImGuiIO &io, uint8_t *data, size_t font_size, const float size, const ImWchar *ranges = 0, const int rasterizer_flags = 0 ) {
			ImFontConfig cfg;

			if ( rasterizer_flags & font_flag_t::MONOHINTING )
				cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_MonoHinting;

			if ( rasterizer_flags & font_flag_t::MONOCHROME )
				cfg.FontBuilderFlags |= ImGuiFreeTypeBuilderFlags_Monochrome;

			m_handle = ImGui::GetIO ( ).Fonts->AddFontFromMemoryTTF ( data, font_size, size, &cfg, ranges );
			m_size.m_height = size;
		}

		// ctor.
		__forceinline c_font ( ImFont *font ) {
			m_handle = font;
			m_size = size ( "A" );
		}

		void string ( float x, float y, color_t color, const std::string &text, string_flags_t flags = ALIGN_LEFT, bool outline = false, bool shadow = true );
		void wstring ( float x, float y, color_t color, const std::wstring &text, string_flags_t flags = ALIGN_LEFT, bool outline = false, bool shadow = true );
		draw_list::font_dim_t size ( const std::string &text );
		font_dim_t wsize ( const std::wstring &text );

		__forceinline void invalidate ( ) {
			m_handle = nullptr;
			m_size = {};
		}
	};

	inline c_font menu;
	inline c_font menu_bold;
	inline c_font menu_small;

	void init ( LPDIRECT3DDEVICE9 device );
	void rect_filled ( float x, float y, float w, float h, color_t color );
	void rect_outlined ( float x, float y, float w, float h, color_t color, float thickness = 1.f );
	void line ( float x, float y, float x0, float y0, color_t color, float thickness = 1.f, bool anti_aliased = true );
	void multi_gradient ( float x, float y, float w, float h, color_t c1, color_t c2, color_t c3, color_t c4 );
	void clip ( float x, float y, float w, float h, const std::function < void ( ) > &fn );
	void draw ( const std::function < void ( ) > &fn );
	void gradient ( float x, float y, float w, float h, color_t c1, color_t c2, bool vertical, bool filled = true );
};
