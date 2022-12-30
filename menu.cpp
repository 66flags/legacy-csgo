#include "includes.h"

gui_t gui { };

void gui_t::init ( ) {
	auto menu_dim = rect_t { 0, 0, 502, 301 };
	auto screen_center = vec2_t { ( render.m_width - menu_dim.w ) / 2, ( render.m_height - menu_dim.h ) / 2 };

	auto window = this->create_window ( _ ( "hypnotic" ), { screen_center.x, screen_center.y, menu_dim.w, menu_dim.h } ); {
		auto rage = window->add_tab ( _ ( "Rage" ) ); {
			auto general = rage->add_child ( _ ( "General" ), { 0, 0, 236, 243 } ); {
			}

			auto antiaim = rage->add_child ( _ ( "Anti-aim" ), { 245, 0, 236, 116 } ); {
			}

			auto fakelag = rage->add_child ( _ ( "Fakelag" ), { 245, 127, 236, 116 } ); {

			}
		}

		auto visuals = window->add_tab ( _ ( "Visuals" ) ); {
			auto players = visuals->add_child ( _ ( "Main" ), { 0, 0, 236, 243 } ); {
				players->add_keybind ( _ ( "Toggle key" ), &settings.get_item < int > ( _ ( "visuals.visual_toggle_key" ) ), &settings.get_item < int > ( _ ( "visuals.visual_toggle_key_toggle" ) ) );
				players->add_checkbox ( _ ( "Enable" ), &settings.get_item < bool > ( _ ( "visuals.players_enable" ) ) );
				players->add_checkbox ( _ ( "Dormant" ), &settings.get_item < bool > ( _ ( "visuals.dormant" ) ) );
				players->add_slider ( _ ( "Dormacy time" ), &settings.get_item < float > ( _ ( "visuals.dormancy_time" ) ), 0, 10 );
				players->add_colorpicker ( _ ( "Health color 1" ), &settings.get_item < color_t > ( _ ( "visuals.health_color1" ) ) );
				players->add_colorpicker ( _ ( "Health color 2" ), &settings.get_item < color_t > ( _ ( "visuals.health_color2" ) ), 20 );
				players->add_checkbox ( _ ( "Health" ), &settings.get_item < bool > ( _ ( "visuals.health" ) ) );
				players->add_colorpicker ( _ ( "Health color 1" ), &settings.get_item < color_t > ( _ ( "visuals.name_color" ) ) );
				players->add_checkbox ( _ ( "Name" ), &settings.get_item < bool > ( _ ( "visuals.name" ) ) );
				players->add_colorpicker ( _ ( "Health color 1" ), &settings.get_item < color_t > ( _ ( "visuals.weapon_color" ) ) );
				players->add_checkbox ( _ ( "Weapon" ), &settings.get_item < bool > ( _ ( "visuals.weapon" ) ) );
				players->add_colorpicker ( _ ( "Health color 1" ), &settings.get_item < color_t > ( _ ( "visuals.box_color" ) ) );
				players->add_checkbox ( _ ( "Bounding box" ), &settings.get_item < bool > ( _ ( "visuals.box" ) ) );
			}

			auto models = visuals->add_child ( _ ( "Models" ), { 245, 0, 236, 116 } ); {
			}

			auto misc = visuals->add_child ( _ ( "Misc" ), { 245, 127, 236, 116 } ); {
				misc->add_colorpicker ( _ ( "Grenade line color" ), &settings.get_item < color_t > ( _ ( "visuals.grenade_line_color" ) ) );
				misc->add_colorpicker ( _ ( "Grenade circle color" ), &settings.get_item < color_t > ( _ ( "visuals.grenade_circle_color" ) ), 20 );
				misc->add_checkbox ( _ ( "Grenade trajectory" ), &settings.get_item < bool > ( _ ( "visuals.grenade_visualization" ) ) );
				misc->add_checkbox ( _ ( "Prop transparency" ), &settings.get_item < bool > ( _ ( "visuals.prop_transparency" ) ) );
				misc->add_slider ( _ ( "Prop transparency amount" ), &settings.get_item < float > ( _ ( "visuals.prop_transparency_amt" ) ), 0.0f, 100.0f );
				misc->add_keybind ( _ ( "Thirdperson key" ), &settings.get_item < int > ( _ ( "visuals.thirdperson_key" ) ), &settings.get_item < int > ( _ ( "visuals.thirdperson_toggle" ) ) );
				misc->add_checkbox ( _ ( "Thirdperson" ), &settings.get_item < bool > ( _ ( "visuals.thirdperson" ) ) );
				misc->add_slider ( _ ( "Thirdperson distance" ), &settings.get_item < float > ( _ ( "visuals.thirdperson_dist" ) ), 0.0f, 120.0f );
				misc->add_checkbox ( _ ( "Force when spectating" ), &settings.get_item < bool > ( _ ( "visuals.thirdperson_spectator" ) ) );
				
				misc->add_multi_combobox ( _ ( "Log events" ),
					{ 
						&settings.get_item < bool > ( _ ( "events.hurt" ) ),
						&settings.get_item < bool > ( _ ( "events.buy" ) ),
						&settings.get_item < bool > ( _ ( "events.bomb" ) ), 
						&settings.get_item < bool > ( _ ( "events.round" ) ) 
					}, { _ ( "Entity hurt" ), _ ( "Weapon buy" ), _ ( "Bomb planted" ), _ ( "Rounds" ) } );
			}
		}

		auto misc = window->add_tab ( _ ( "Misc" ) ); {
			auto movement = misc->add_child ( _ ( "Movement" ), { 0, 0, 236, 243 } ); {
				movement->add_checkbox ( _ ( "Auto bhop" ), &settings.get_item < bool > ( _ ( "misc.bunny_hop" ) ) );
				movement->add_checkbox ( _ ( "Auto strafe" ), &settings.get_item < bool > ( _ ( "misc.auto_strafe" ) ) );
				movement->add_combobox ( _ ( "Strafe type" ), &settings.get_item < int > ( _ ( "misc.auto_strafe_type" ) ), { _ ( "View angle" ), _ ( "Key strafe" ) } );
				movement->add_checkbox ( _ ( "Unlimited duck" ), &settings.get_item < bool > ( _ ( "misc.unlimited_duck" ) ) );
			}

			auto other = misc->add_child ( _ ( "Other" ), { 245, 0, 236, 243 } ); {
				other->add_button ( _ ( "Reload default values" ), [ ] {
					settings.init ( );
				} );
			}
		}
	}
}

void gui_t::draw ( ) {
	inputsys.update ( );

	if ( inputsys.key_pressed ( VK_INSERT ) ) {
		this->m_visible = !this->m_visible;
		m_should_save = true;
	}
	
	if ( m_should_save ) {
		m_should_save = false;
		settings.save ( g.m_cfg_name );
	}
	
	if ( !this->m_visible ) {
		return;
	}

	for ( auto &window : this->m_windows ) {
		window->think ( );
		window->paint ( );
	}
}

IDirect3DStateBlock9 *pixel_state = NULL; IDirect3DVertexDeclaration9 *vertDec; IDirect3DVertexShader9 *vertShader;
DWORD dwOld_D3DRS_COLORWRITEENABLE;
DWORD srgbwrite;

void gui_t::apply_render_states ( IDirect3DDevice9 *device ) {
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

void gui_t::restore_render_states ( IDirect3DDevice9 *device ) {
	pixel_state->Apply ( );
	pixel_state->Release ( );

	device->SetRenderState ( D3DRS_COLORWRITEENABLE, dwOld_D3DRS_COLORWRITEENABLE );
	device->SetRenderState ( D3DRS_SRGBWRITEENABLE, srgbwrite );
}

void gui_t::begin ( IDirect3DDevice9 *device, const std::function< void ( ) > &fn ) {
	apply_render_states ( device );
	fn ( );
	restore_render_states ( device );
}