#include "includes.h"

menu_t menu { };

std::wstring config_name = L"default";
std::mutex ui_mutex;

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
int main_dpi_scale = 0;

void unlock_hidden_convars ( ) {
	if ( !interfaces::m_cvar )
		return;

	cvar_t *list;

	list = interfaces::m_cvar->get_commands ( );

	if ( !list )
		return;

	int i = 0;

	for ( auto it = list; it != nullptr; it = it->m_next ) {
		it->m_flags &= ~( ( 1 << 4 ) | ( 1 << 1 ) ); /* FVAR_HIDDEN | FVAR_DEVELOPMENTONLY */
		i++;
	}

	notify.print ( tfm::format ( _ ( "Unlocked %d convars.\n" ), i ).c_str ( ) );
}

void menu_t::draw ( ) {
	draw_list::frametime = interfaces::m_globals->m_frametime;
	sesui::begin_frame ( _ ( L"Counter-Strike: Global Offensive" ) );

	if ( sesui::input::key_pressed ( VK_INSERT ) )
		m_opened = !m_opened;

	if ( sesui::begin_window ( _ ( L"hypnotic" ), sesui::rect ( ( render.m_width - 639 ) / 2, ( render.m_height - 488 ) / 2, 639, 488 ), m_opened, sesui::window_flags::no_closebutton ) ) {
		if ( sesui::begin_tabs ( 5 ) ) {
			sesui::tab ( _ ( L"rage" ), main_window_tab_idx );
			sesui::tab ( _ ( L"visuals" ), main_window_tab_idx );
			sesui::tab ( _ ( L"misc" ), main_window_tab_idx );
			sesui::tab ( _ ( L"skins" ), main_window_tab_idx );
			sesui::tab ( _ ( L"cheat" ), main_window_tab_idx );
			sesui::end_tabs ( );
		}

		switch ( main_window_tab_idx ) {
		case 0: {
			if ( sesui::begin_group ( _ ( L"weapon selection" ), sesui::rect ( 0.0f, 0.0f, 0.5f, 0.27f ), sesui::rect ( 0.0f, 0.0f, -sesui::style.spacing * 0.5f, 0.0f ) ) ) {
				sesui::end_group ( );
			}

			if ( sesui::begin_group ( _ ( L"config" ), sesui::rect ( 0.0f, 0.30f, 0.5f, 0.70f ), sesui::rect ( 0.0f, 0.0f, -sesui::style.spacing * 0.5f, 0.0f ) ) ) {
				sesui::end_group ( );
			}

			if ( sesui::begin_group ( _ ( L"anti aim" ), sesui::rect ( 0.5f, 0.0f, 0.5f, 1.0f ), sesui::rect ( sesui::style.spacing * 0.5f, 0.0f, -sesui::style.spacing * 0.5f, 0.0f ) ) ) {
				sesui::end_group ( );
			}
		} break;
		case 1: {
			if ( sesui::begin_group ( _ ( L"player esp" ), sesui::rect ( 0.0f, 0.0f, 0.5f, 1.0f ), sesui::rect ( 0.0f, 0.0f, -sesui::style.spacing * 0.5f, 0.0f ) ) ) {
				sesui::checkbox ( _ ( L"Enable" ), settings.get_item < bool > ( _ ( "visuals.player_enable" ) ) );
				sesui::checkbox ( _ ( L"Dormant" ), settings.get_item < bool > ( _ ( "visuals.player_dormant" ) ) );
				sesui::checkbox ( _ ( L"Bounding box" ), settings.get_item < bool > ( _ ( "visuals.player_box" ) ) );
				sesui::same_line ( );
				sesui::colorpicker ( _ ( L"Color##box color" ), settings.get_item < sesui::color > ( _ ( "visuals.player_box_color" ) ) );

				sesui::multiselect ( _ ( L"Flags" ), { 
					{ _ ( L"Armor" ), settings.get_item < bool > ( _ ( "visuals.flags_armor" ) ) },
					{ _ ( L"Money" ), settings.get_item < bool > ( _ ( "visuals.flags_money" ) ) }, 
					{ _ ( L"Bomb" ), settings.get_item < bool > ( _ ( "visuals.flags_bomb" ) ) },
					{ _ ( L"Defuser" ), settings.get_item < bool > ( _ ( "visuals.flags_defuser" ) ) }, 
					{ _ ( L"Scoped" ), settings.get_item < bool > ( _ ( "visuals.flags_scoped" ) ) },
					{ _ ( L"Distance" ), settings.get_item < bool > ( _ ( "visuals.flags_distance" ) ) }
				} );

				sesui::checkbox ( _ ( L"Name" ), settings.get_item < bool > ( _ ( "visuals.player_name" ) ) );
				sesui::same_line ( );
				sesui::colorpicker ( _ ( L"Color##name color" ), settings.get_item < sesui::color > ( _ ( "visuals.player_name_color" ) ) );
				sesui::checkbox ( _ ( L"Health" ), settings.get_item < bool > ( _ ( "visuals.player_health" ) ) );
				sesui::multiselect ( _ ( L"Weapon" ), { { _ ( L"Name" ), settings.get_item < bool > ( _ ( "visuals.player_weapon_name" ) ) }, { _ ( L"Icon" ), settings.get_item < bool > ( _ ( "visuals.player_weapon_icon" ) ) } } );
				sesui::checkbox ( _ ( L"Ammo" ), settings.get_item < bool > ( _ ( "visuals.player_ammo" ) ) );
				sesui::same_line ( );
				sesui::colorpicker ( _ ( L"Color##ammo color" ), settings.get_item < sesui::color > ( _ ( "visuals.player_ammo_color" ) ) );
				sesui::checkbox ( _ ( L"LBY indication" ), settings.get_item < bool > ( _ ( "visuals.player_lby_indicator" ) ) );
				sesui::same_line ( );
				sesui::colorpicker ( _ ( L"Color##lby indication" ), settings.get_item < sesui::color > ( _ ( "visuals.player_lby_indicator_color" ) ) );

				sesui::end_group ( );
			}

			if ( sesui::begin_group ( _ ( L"other" ), sesui::rect ( 0.5f, 0.0f, 0.5f, 0.47f ), sesui::rect ( sesui::style.spacing * 0.5f, 0.0f, -sesui::style.spacing * 0.5f, 0.0f ) ) ) {
				sesui::checkbox ( _ ( L"Transparent props" ), settings.get_item < bool > ( _ ( "visuals.transparent_props" ) ) );

				if ( settings.get_item < bool > ( _ ( "visuals.transparent_props" ) ) )
					sesui::slider ( _ ( L"Amount##transparent_props" ), settings.get_item < int > ( _ ( "visuals.transparent_props_amount" ) ), 0, 100, _ ( L"%d" ) );

				sesui::checkbox ( _ ( L"Modulate world" ), settings.get_item < bool > ( _ ( "visuals.modulate_world" ) ) );
				sesui::same_line ( );
				sesui::colorpicker ( _ ( L"Color##modulate world" ), settings.get_item < sesui::color > ( _ ( "visuals.modulate_world_color" ) ) );
				sesui::combobox ( _ ( L"Skybox" ), settings.get_item < int > ( _ ( "visuals.skybox" ) ), 
					{ 
						_ ( L"cs_baggage_skybox_" ) ,
						_ ( L"cs_tibet" ) ,
						_ ( L"vietnam" ) ,
						_ ( L"sky_lunacy" ) ,
						_ ( L"embassy" ) ,
						_ ( L"italy" ) ,
						_ ( L"jungle" ) ,
						_ ( L"office" ) ,
						_ ( L"sky_cs15_daylight01_hdr" ) ,
						_ ( L"sky_cs15_daylight02_hdr" ) ,
						_ ( L"sky_day02_05" ) ,
						_ ( L"sky_csgo_cloudy01" ) ,
						_ ( L"sky_csgo_night02" ) ,
						_ ( L"sky_csgo_night02b" ) ,
					} );

				sesui::checkbox ( _ ( L"Thirdperson" ), settings.get_item < bool > ( _ ( "visuals.thirdperson" ) ) );
				sesui::keybind ( _ ( L"Thirdperson key##thirdperson" ), settings.get_item < int > ( _ ( "visuals.thirdperson_key" ) ), settings.get_item < int > ( _ ( "visuals.thirdperson_key_toggle" ) ) );
				sesui::slider ( _ ( L"Thirdperson distance##thirdpersondist" ), settings.get_item < int > ( _ ( "visuals.thirdperson_distance" ) ), 0, 160, _ ( L"%d" ) );
				

				sesui::end_group ( );
			}

			if ( sesui::begin_group ( _ ( L"models" ), sesui::rect ( 0.5f, 0.50f, 0.5f, 0.50f ), sesui::rect ( sesui::style.spacing * 0.5f, 0.0f, -sesui::style.spacing * 0.5f, 0.0f ) ) ) {
				sesui::end_group ( );
			}
		} break;
		case 2: {
			if ( sesui::begin_group ( _ ( L"main" ), sesui::rect ( 0.0f, 0.0f, 0.5f, 1.0f ), sesui::rect ( 0.0f, 0.0f, -sesui::style.spacing * 0.5f, 0.0f ) ) ) {
				sesui::checkbox ( _ ( L"Auto bhop" ), settings.get_item < bool > ( _ ( "misc.bunny_hop" ) ) );
				sesui::checkbox ( _ ( L"Auto strafe" ), settings.get_item < bool > ( _ ( "misc.auto_strafe" ) ) );

				if ( settings.get_item < bool > ( _ ( "misc.auto_strafe" ) ) )
					sesui::combobox ( _ ( L"Strafe type" ), settings.get_item < int > ( _ ( "misc.auto_strafe_type" ) ), { _ ( L"View angles" ), _ ( L"Movement keys" ) } );
				
				sesui::checkbox ( _ ( L"No crouch cooldown" ), settings.get_item < bool > ( _ ( "misc.unlimited_duck" ) ) );
				
				sesui::end_group ( );
			}

			if ( sesui::begin_group ( _ ( L"hack" ), sesui::rect ( 0.5f, 0.0f, 0.5f, 1.0f ), sesui::rect ( sesui::style.spacing * 0.5f, 0.0f, -sesui::style.spacing * 0.5f, 0.0f ) ) ) {
				sesui::combobox ( _ ( L"DPI scale" ), main_dpi_scale, { _ ( L"100%" ), _ ( L"125%" ), _ ( L"150%" ), _ ( L"175%" ), _ ( L"200%" ) } ); {
					switch ( main_dpi_scale ) {
					case 0: sesui::globals::dpi = 1.0f; break;
					case 1: sesui::globals::dpi = 1.25f; break;
					case 2: sesui::globals::dpi = 1.5f; break;
					case 3: sesui::globals::dpi = 1.75f; break;
					case 4: sesui::globals::dpi = 2.0f; break;
					}
				}

				if ( sesui::button ( _ ( L"Unlock hidden convars" ) ) ) {
					unlock_hidden_convars ( );
					interfaces::m_engine->execute_client_cmd ( _ ( "play ui\\buttonclick" ) );
				}

				sesui::end_group ( );
			}
		} break;
		case 3: {
			/* EMPTY. */
		} break;
		case 4: {
			if ( sesui::begin_group ( _ ( L"configs" ), sesui::rect ( 0.0f, 0.0f, 0.5f, 1.0f ), sesui::rect ( 0.0f, 0.0f, -sesui::style.spacing * 0.5f, 0.0f ) ) ) {
				for ( auto &item : settings.config_list ) {
					if ( sesui::button ( item.data ( ) ) )
						config_name = item;
				}

				sesui::end_group ( );
			}

			if ( sesui::begin_group ( _ ( L"options" ), sesui::rect ( 0.5f, 0.0f, 0.5f, 1.0f ), sesui::rect ( sesui::style.spacing * 0.5f, 0.0f, -sesui::style.spacing * 0.5f, 0.0f ) ) ) {
				sesui::textbox ( _ ( L"Config name" ), config_name );

				if ( sesui::button ( _ ( L"Save" ) ) ) {
					ui_mutex.lock ( );
					settings.config_list.clear ( );
					settings.save ( std::wstring_convert < std::codecvt_utf8 < wchar_t > > ( ).to_bytes ( config_name ) );
					settings.refresh ( );
					ui_mutex.unlock ( );

					interfaces::m_engine->execute_client_cmd ( _ ( "play ui\\buttonclick" ) );
				};

				if ( sesui::button ( _ ( L"Load" ) ) ) {
					ui_mutex.lock ( );
					settings.load ( std::wstring_convert < std::codecvt_utf8 < wchar_t > > ( ).to_bytes ( config_name ) );
					ui_mutex.unlock ( );

					interfaces::m_engine->execute_client_cmd ( _ ( "play ui\\buttonclick" ) );
				};

				if ( sesui::button ( _ ( L"Delete" ) ) ) {
					ui_mutex.lock ( );

					std::remove ( std::string ( std::wstring_convert < std::codecvt_utf8 < wchar_t > > ( ).to_bytes ( config_name + _ ( L".json" ) ) ).data ( ) );
					settings.config_list.clear ( );
					settings.refresh ( );

					ui_mutex.unlock ( );

					interfaces::m_engine->execute_client_cmd ( _ ( "play ui\\buttonclick" ) );
				};

				if ( sesui::button ( _ ( L"Refresh" ) ) ) {
					ui_mutex.lock ( );

					settings.config_list.clear ( );
					settings.refresh ( );

					ui_mutex.unlock ( );

					interfaces::m_engine->execute_client_cmd ( _ ( "play ui\\buttonclick" ) );
				};
				sesui::end_group ( );
			}
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