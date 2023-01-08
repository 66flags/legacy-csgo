#include "includes.h"

menu_t menu { };

void menu_t::init ( ) {
	draw_list::device = interfaces::m_device;

	unsigned long font_count = 0;
	LI_FN ( AddFontMemResourceEx ) ( &sesui::fonts::font_default, sizeof sesui::fonts::font_default, nullptr, &font_count );

	sesui::draw_list.draw_polygon = draw_list::polygon;
	sesui::draw_list.draw_text = draw_list::text;
	sesui::draw_list.get_text_size = draw_list::get_text_size;
	sesui::draw_list.draw_multicolor_polygon = draw_list::multicolor_polygon;
	sesui::draw_list.get_frametime = draw_list::get_frametime;
	sesui::draw_list.begin_clip = draw_list::begin_clip;
	sesui::draw_list.end_clip = draw_list::end_clip;
	sesui::draw_list.create_font = draw_list::create_font;
}

int main_window_tab_idx = 0;

void menu_t::draw ( ) {
	draw_list::frametime = interfaces::m_globals->m_frametime;
	sesui::begin_frame ( _ ( L"Counter-Strike: Global Offensive" ) );

	if ( sesui::input::key_pressed ( VK_INSERT ) )
		m_opened = !m_opened;

	if ( sesui::begin_window ( _ ( L"hypnotic" ), sesui::rect ( ( render.m_width - 639 ) / 2, ( render.m_height - 488 ) / 2, 639, 488 ), m_opened, sesui::window_flags::no_closebutton ) ) {
		if ( sesui::begin_tabs ( 6 ) ) {
			sesui::tab ( _ ( L"rage" ), main_window_tab_idx );
			sesui::tab ( _ ( L"visuals" ), main_window_tab_idx );
			sesui::tab ( _ ( L"misc" ), main_window_tab_idx );
			sesui::tab ( _ ( L"players" ), main_window_tab_idx );
			sesui::tab ( _ ( L"skins" ), main_window_tab_idx );
			sesui::tab ( _ ( L"cheat" ), main_window_tab_idx );
			sesui::end_tabs ( );
		}

		switch ( main_window_tab_idx ) {
		case 0: {
			if ( sesui::begin_group ( _ ( L"weapon selection" ), sesui::rect ( 0.0f, 0.0f, 0.5f, 0.27f ), sesui::rect ( 0.0f, 0.0f, -sesui::style.spacing * 0.5f, 0.0f ) ) ) {
				sesui::end_group ( );
			}

			if ( sesui::begin_group ( _ ( L"configuration" ), sesui::rect ( 0.0f, 0.30f, 0.5f, 0.70f ), sesui::rect ( 0.0f, 0.0f, -sesui::style.spacing * 0.5f, 0.0f ) ) ) {
				sesui::end_group ( );
			}

			if ( sesui::begin_group ( L"anti-aim", sesui::rect ( 0.5f, 0.0f, 0.5f, 1.0f ), sesui::rect ( sesui::style.spacing * 0.5f, 0.0f, -sesui::style.spacing * 0.5f, 0.0f ) ) ) {
				sesui::end_group ( );
			}
		} break;
		case 1: {

		} break;
		case 2: {

		} break;
		}

		sesui::end_window ( );
	}

	sesui::render ( );
	sesui::end_frame ( );
}

IDirect3DStateBlock9 *pixel_state = NULL; IDirect3DVertexDeclaration9 *vertDec; IDirect3DVertexShader9 *vertShader;
DWORD dwOld_D3DRS_COLORWRITEENABLE;
DWORD srgbwrite;

void menu_t::apply_render_states ( IDirect3DDevice9 *device ) {
	D3DVIEWPORT9 d3d_viewport;

	device->GetViewport ( &d3d_viewport );
	device->CreateStateBlock ( D3DSBT_ALL, &pixel_state );
	pixel_state->Capture ( );

	device->GetRenderState ( D3DRS_COLORWRITEENABLE, &dwOld_D3DRS_COLORWRITEENABLE );
	device->GetRenderState ( D3DRS_SRGBWRITEENABLE, &srgbwrite );

	device->SetVertexShader ( nullptr );
	device->SetPixelShader ( nullptr );
	device->SetFVF ( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );

	device->SetRenderState ( D3DRS_LIGHTING, FALSE );
	device->SetRenderState ( D3DRS_FOGENABLE, FALSE );
	device->SetRenderState ( D3DRS_CULLMODE, D3DCULL_NONE );
	device->SetRenderState ( D3DRS_FILLMODE, D3DFILL_SOLID );
	device->SetRenderState ( D3DRS_ZENABLE, D3DZB_FALSE );
	device->SetRenderState ( D3DRS_SCISSORTESTENABLE, TRUE );
	device->SetRenderState ( D3DRS_ZWRITEENABLE, FALSE );
	device->SetRenderState ( D3DRS_STENCILENABLE, FALSE );
	device->SetRenderState ( D3DRS_MULTISAMPLEANTIALIAS, TRUE );
	device->SetRenderState ( D3DRS_ANTIALIASEDLINEENABLE, TRUE );
	device->SetRenderState ( D3DRS_ALPHABLENDENABLE, TRUE );
	device->SetRenderState ( D3DRS_ALPHATESTENABLE, FALSE );
	device->SetRenderState ( D3DRS_SEPARATEALPHABLENDENABLE, TRUE );
	device->SetRenderState ( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
	device->SetRenderState ( D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA );
	device->SetRenderState ( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
	device->SetRenderState ( D3DRS_DESTBLENDALPHA, D3DBLEND_ONE );
	device->SetRenderState ( D3DRS_SRGBWRITEENABLE, FALSE );
	device->SetRenderState ( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );
}

void menu_t::restore_render_states ( IDirect3DDevice9 *device ) {
	pixel_state->Apply ( );
	pixel_state->Release ( );

	device->SetRenderState ( D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE );
	device->SetRenderState ( D3DRS_SRGBWRITEENABLE, srgbwrite );
}

void menu_t::begin ( IDirect3DDevice9 *device, const std::function< void ( ) > &fn ) {
	apply_render_states ( device );
	fn ( );
	restore_render_states ( device );
}