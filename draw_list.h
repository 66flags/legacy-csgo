#pragma once

namespace draw_list {
	extern float frametime;
	extern IDirect3DDevice9 *device;

	void create_font ( sesui::font &font, bool force ) noexcept;
	void polygon ( const std::vector< sesui::vec2 > &verticies, const sesui::color &color, bool filled ) noexcept;
	void multicolor_polygon ( const std::vector< sesui::vec2 > &verticies, const std::vector< sesui::color > &colors, bool filled ) noexcept;
	void text ( const sesui::vec2 &pos, const sesui::font &font, const sesui::ses_string &text, const sesui::color &color ) noexcept;
	void get_text_size ( const sesui::font &font, const sesui::ses_string &text, sesui::vec2 &dim_out ) noexcept;
	float get_frametime ( ) noexcept;
	void begin_clip ( const sesui::rect &region ) noexcept;
	void end_clip ( ) noexcept;
}