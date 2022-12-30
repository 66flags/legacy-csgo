#include "includes.h"

void draw_list::init ( LPDIRECT3DDEVICE9 device ) {
	interfaces::m_engine->get_screen_size ( render.m_width, render.m_height );

	if ( !device )
		return;

	/* init imgui. */
	ImGui::CreateContext ( );
	ImGuiIO &io = ImGui::GetIO ( ); ( void ) io;
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark ( );
	//ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	ImGui_ImplWin32_Init ( FindWindowA ( _ ( "Valve001" ), nullptr ) );
	ImGui_ImplDX9_Init ( device );

	/* init font data. */
	menu = c_font ( io, visitor_ttf, sizeof ( visitor_ttf ), 9.5f, io.Fonts->GetGlyphRangesJapanese ( ), font_flag_t::MONOCHROME | font_flag_t::MONOHINTING );
	menu_bold = c_font ( io, "C:\\Windows\\Fonts\\verdanab.ttf", 11.5f, io.Fonts->GetGlyphRangesJapanese ( ), font_flag_t::MONOCHROME | font_flag_t::MONOHINTING );
	menu_small = c_font ( io, small_fonts_ttf, sizeof ( small_fonts_ttf ), 8.5f, io.Fonts->GetGlyphRangesJapanese ( ), font_flag_t::MONOCHROME | font_flag_t::MONOHINTING );
}

void draw_list::rect_filled ( float x, float y, float w, float h, color_t color ) {
	ImGui::GetBackgroundDrawList ( )->AddRectFilled ( { ImFloor ( x ), ImFloor ( y ) }, { ImFloor ( x + w ), ImFloor ( y + h ) }, ImColor { color.r, color.g, color.b, color.a } );
}

void draw_list::rect_outlined ( float x, float y, float w, float h, color_t color, float thickness /*= 1.f*/ ) {
	ImGui::GetBackgroundDrawList ( )->AddRect ( { ImFloor ( x ), ImFloor ( y ) }, { ImFloor ( x + w ), ImFloor ( y + h ) }, ImColor { color.r, color.g, color.b, color.a }, 0.0f, 15, thickness );
}

void draw_list::gradient ( float x, float y, float w, float h, color_t c1, color_t c2, bool vertical, bool filled /* = true */ ) {
	ImGui::GetBackgroundDrawList ( )->AddRectFilledMultiColor ( { ImFloor ( x ), ImFloor ( y ) }, { ImFloor ( x + w ), ImFloor ( y + h ) },
		// upper left
		ImColor { c1.r, c1.g, c1.b, c1.a },
		// upper right
		vertical ? ImColor { c1.r, c1.g, c1.b, c1.a } : ImColor { c2.r, c2.g, c2.b, c2.a },
		// bottom right
		ImColor { c2.r, c2.g, c2.b, c2.a },
		// bottom left
		vertical ? ImColor { c2.r, c2.g, c2.b, c2.a } : ImColor { c1.r, c1.g, c1.b, c1.a } );
}

void draw_list::line ( float x, float y, float x0, float y0, color_t color, float thickness /*= 1.f*/, bool anti_aliased /*= true*/ ) {
	ImGui::GetBackgroundDrawList ( )->AddLine ( { ImFloor ( x ), ImFloor ( y ) }, { ImFloor ( x0 ), ImFloor ( y0 ) }, ImColor { color.r, color.g, color.b, color.a }, thickness );
}

void draw_list::multi_gradient ( float x, float y, float w, float h, color_t c1, color_t c2, color_t c3, color_t c4 ) {
	ImGui::GetBackgroundDrawList ( )->AddRectFilledMultiColor ( { ImFloor ( x ), ImFloor ( y ) }, { ImFloor ( x + w ), ImFloor ( y + h ) },
		// upper left
		ImColor { c1.r, c1.g, c1.b, c1.a },
		// upper right
		ImColor { c2.r, c2.g, c2.b, c2.a },
		// bottom right
		ImColor { c3.r, c3.g, c3.b, c3.a },
		// bottom left
		ImColor { c4.r, c4.g, c4.b, c4.a } );
}

void draw_list::c_font::wstring ( float x, float y, color_t color, const std::wstring &text, string_flags_t flags /*= draw_list::ALIGN_LEFT */, bool outline /*= false*/, bool shadow /*= false*/ ) {
	string ( x, y, color, std::string ( text.begin ( ), text.end ( ) ), flags, outline );
}

void draw_list::clip ( float x, float y, float w, float h, const std::function < void ( ) > &fn ) {
	ImGui::GetBackgroundDrawList ( )->PushClipRect ( { ImFloor ( x ), ImFloor ( y ) }, { ImFloor ( x + w ), ImFloor ( y + h ) }, true );

	fn ( );

	ImGui::GetBackgroundDrawList ( )->PopClipRect ( );
}

void draw_list::draw ( const std::function < void ( ) > &fn ) {
	ImGui::GetBackgroundDrawList ( )->PushClipRectFullScreen ( );

	ImGui_ImplDX9_NewFrame ( );
	ImGui_ImplWin32_NewFrame ( );
	ImGui::NewFrame ( );

	fn ( );

	ImGui::Render ( );
	ImGui_ImplDX9_RenderDrawData ( ImGui::GetDrawData ( ) );
}

void draw_list::c_font::string ( float x, float y, color_t color, const std::string &text, string_flags_t flags /*= draw_list::ALIGN_LEFT */, bool outline /*= false*/, bool shadow /*= false*/ ) {
	ImGui::PushFont ( this->m_handle );

	font_dim_t text_size = size ( text );

	if ( flags & draw_list::ALIGN_RIGHT )
		x -= text_size.m_width;
	if ( flags & draw_list::ALIGN_CENTER )
		x -= text_size.m_width / 2;

	if ( shadow )
		ImGui::GetBackgroundDrawList ( )->AddText ( { ImFloor ( x + 1 ), ImFloor ( y + 1 ) }, ImColor { 0, 0, 0, color.a }, text.c_str ( ) );

	ImGui::GetBackgroundDrawList ( )->AddText ( { ImFloor ( x ), ImFloor ( y ) }, ImColor { color.r, color.g, color.b, color.a }, text.c_str ( ) );

	ImGui::PopFont ( );
}

draw_list::font_dim_t draw_list::c_font::size ( const std::string &text ) {
	ImVec2 size = this->m_handle->CalcTextSizeA ( m_size.m_height, FLT_MAX, 0.0f, text.data ( ) );

	font_dim_t ret = { size.x, size.y };

	return ret;
}

draw_list::font_dim_t draw_list::c_font::wsize ( const std::wstring &text ) {
	return size ( std::string ( text.begin ( ), text.end ( ) ) );
}