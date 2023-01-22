#include "includes.h"

const char *sky_names [ ] = {
	_ ( "cs_baggage_skybox_" ) ,
	_ ( "cs_tibet" ) ,
	_ ( "vietnam" ) ,
	_ ( "sky_lunacy" ) ,
	_ ( "embassy" ) ,
	_ ( "italy" ) ,
	_ ( "jungle" ) ,
	_ ( "office" ) ,
	_ ( "sky_cs15_daylight01_hdr" ) ,
	_ ( "sky_cs15_daylight02_hdr" ) ,
	_ ( "sky_day02_05" ) ,
	_ ( "sky_csgo_cloudy01" ) ,
	_ ( "sky_csgo_night02" ) ,
	_ ( "sky_csgo_night02b" ) ,
};

void __fastcall hooks_t::frame_stage_notify ( void *ecx, void *edx, frame_stage_t stage ) {
	if ( stage != frame_stage_t::start )
		g.m_stage = stage;

	g.m_local = ( interfaces::m_engine->is_connected ( ) || interfaces::m_engine->is_in_game ( ) ) ? interfaces::m_entlist->get< player_t * > ( interfaces::m_engine->get_local_player ( ) ) : nullptr;
	
	static cvar_t *r_DrawSpecificStaticProp = interfaces::m_cvar->find_var ( HASH ( "r_DrawSpecificStaticProp" ) );

	if ( stage == frame_stage_t::render_start ) {	
		static int last_skybox = settings.get_item < int > ( _ ( "visuals.skybox" ) );
		static sesui::color last_world_color = settings.get_item < sesui::color > ( _ ( "visuals.modulate_world_color" ) );

		if ( last_skybox != settings.get_item < int > ( _ ( "visuals.skybox" ) ) ) {
			static auto r_loadnamedsky = pattern::find ( _ ( "engine.dll" ), _ ( "55 8B EC 81 ? ? ? ? ? 56 57 8B F9 C7" ) ).as < bool ( __thiscall * ) ( const char * ) > ( );

			r_loadnamedsky ( sky_names [ settings.get_item < int > ( _ ( "visuals.skybox" ) ) ] );
			last_skybox = settings.get_item < int > ( _ ( "visuals.skybox" ) );
		}

		static auto &modulate_world_color = settings.get_item < sesui::color > ( _ ( "visuals.modulate_world_color" ) );

		if ( settings.get_item < bool > ( _ ( "visuals.modulate_world" ) ) ) {
			if ( r_DrawSpecificStaticProp->get_int ( ) != 0 )
				r_DrawSpecificStaticProp->set_int ( 0 );

			if ( last_world_color.r != modulate_world_color.r || last_world_color.g != modulate_world_color.g || last_world_color.b != modulate_world_color.b ) {
				for ( uint16_t h { interfaces::m_material_sys->first_material ( ) }; h != interfaces::m_material_sys->invalid_material ( ); h = interfaces::m_material_sys->next_material ( h ) ) {
					auto mat = interfaces::m_material_sys->get_material ( h );

					if ( !mat )
						continue;

					if ( strstr ( mat->get_texture_group_name ( ), _ ( "World textures" ) ) ) {
						mat->color_modulate ( modulate_world_color.r * 255.f, modulate_world_color.g * 255.f, modulate_world_color.b * 255.f );
					}
				}

				last_world_color = modulate_world_color;
			}
		}
		else {
			if ( r_DrawSpecificStaticProp->get_int ( ) != -1 )
				r_DrawSpecificStaticProp->set_int ( -1 );
		}

		if ( settings.get_item < bool > ( _ ( "visuals.transparent_props" ) ) ) {
			if ( r_DrawSpecificStaticProp->get_int ( ) != 0 )
				r_DrawSpecificStaticProp->set_int ( 0 );
		}
		else {
			if ( r_DrawSpecificStaticProp->get_int ( ) != -1 )
				r_DrawSpecificStaticProp->set_int ( -1 );
		}
	}

	hooks.m_frame_stage_notify.get_old_method < decltype ( &frame_stage_notify ) > ( )( ecx, edx, stage );
}