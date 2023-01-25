#include "includes.h"

visuals_t visuals { };

void visuals_t::paint ( ) {
	if ( !g.m_local )
		return;

	for ( int i = 1; i <= interfaces::m_entlist->get_highest_entity_index ( ); i++ ) {
		auto ent = interfaces::m_entlist->get < entity_t * > ( i );

		if ( !ent )
			continue;

		draw ( ent );
	}
}

void visuals_t::draw ( entity_t *ent ) {
	static auto &player_enable = settings.get_item < bool > ( _ ( "visuals.player_enable" ) );

	if ( ent->is_player ( ) ) {
		auto pl = ent->as < player_t * > ( );

		if ( !pl->alive ( ) || pl->team ( ) == g.m_local->team ( ) )
			return;

		if ( player_enable )
			player ( pl );
	}
}

void visuals_t::player ( player_t *pl ) {
	static auto &player_box = settings.get_item < bool > ( _ ( "visuals.player_box" ) );
	static auto &player_box_color = settings.get_item < sesui::color > ( _ ( "visuals.player_box_color" ) );
	static auto &player_name = settings.get_item < bool > ( _ ( "visuals.player_name" ) );
	static auto &player_health = settings.get_item < bool > ( _ ( "visuals.player_health" ) );
	static auto &player_name_color = settings.get_item < sesui::color > ( _ ( "visuals.player_name_color" ) );
	static auto &player_weapon_name = settings.get_item < bool > ( _ ( "visuals.player_weapon_name" ) );
	static auto &player_flags_armor = settings.get_item < bool > ( _ ( "visuals.flags_armor" ) );
	static auto &player_flags_money = settings.get_item < bool > ( _ ( "visuals.flags_money" ) );
	static auto &player_flags_bomb = settings.get_item < bool > ( _ ( "visuals.flags_bomb" ) );
	static auto &player_flags_defuser = settings.get_item < bool > ( _ ( "visuals.flags_defuser" ) );
	static auto &player_flags_scoped = settings.get_item < bool > ( _ ( "visuals.flags_scoped" ) );
	static auto &player_flags_distance = settings.get_item < bool > ( _ ( "visuals.flags_distance" ) );

	auto weapon = pl->weapon ( );

	if ( !weapon )
		return;

	auto data = weapon->data ( );

	if ( !data )
		return;

	box_t box;
	if ( !get_player_box ( pl, box ) ) {
		/* offscreen esp. */
		return;
	}

	player_info_t info;
	if ( !interfaces::m_engine->get_player_info ( pl->idx ( ), &info ) )
		return;

	if ( player_weapon_name ) {
		auto name = std::wstring ( weapon->name ( ) );

		std::transform ( name.begin ( ), name.end ( ), name.begin ( ), ::toupper );
		std::string str ( name.begin ( ), name.end ( ) );
		auto dim = render.get_text_size ( fonts [ fonts_t::font_esp_small ].m_data, str.data ( ) );

		render.string ( fonts [ fonts_t::font_esp_small ].m_data, box.x + ( box.w / 2 ) - dim.x / 2, box.y + box.h + 1, { 255, 255, 255, 200 }, str.data ( ) );
	}

	if ( player_box ) {
		render.outlined_rect ( box.x - 1, box.y - 1, box.w + 2, box.h + 2, { 0, 0, 0, 200 } );
		render.outlined_rect ( box.x, box.y, box.w, box.h, { static_cast < int > ( player_box_color.r * 255.f ), static_cast < int > ( player_box_color.g * 255.f ), static_cast < int > ( player_box_color.b * 255.f ), 200 } );
		render.outlined_rect ( box.x + 1, box.y + 1, box.w - 2, box.h - 2, { 0, 0, 0, 200 } );
	}

	if ( player_health ) {
		int y = box.y + 1;
		int h = box.h - 2;

		int health = min ( 100, pl->health ( ) );

		float r = 255.f - ( health * 2.55f );
		float g = health * 2.55f;

		int delta = ( ( h * health ) / 100 );

		render.filled_rect ( box.x - 6, y - 1, 4, h + 2, { 10, 10, 10, 175 } );
		render.filled_rect ( box.x - 5, y + ( h - delta ), 2, delta, { static_cast< int > ( r ), static_cast< int > ( g ), 0, 200 } );

		if ( health < 100 ) {
			auto dim = render.get_text_size ( fonts [ fonts_t::font_esp_small ].m_data, std::to_string ( health ).data ( ) );

			render.string ( fonts [ fonts_t::font_esp_small ].m_data, box.x - 5 - dim.x / 2, y + ( h - delta ) - dim.y / 2, { 255, 255, 255, 200 }, std::to_string ( health ).data ( ) );
		}
	}

	if ( player_name ) {
		auto dim = render.get_text_size ( fonts [ fonts_t::font_esp_name ].m_data, info.m_name );
		
		render.string ( fonts [ fonts_t::font_esp_name ].m_data, box.x + ( box.w / 2 ) - ( dim.x / 2 ), box.y - dim.y, { static_cast < int > ( player_name_color.r * 255.f ), static_cast < int > ( player_name_color.g * 255.f ), static_cast < int > ( player_name_color.b * 255.f ), 200 }, info.m_name );
	}

	/* render flags. */ {
		std::deque < std::pair < const char *, color_t > > m_flags;

		if ( player_flags_money ) {
			char buffer [ 256 ];
			sprintf_s ( buffer, "$%i", pl->account ( ) );
			m_flags.push_back ( { buffer, { 150, 200, 60 } } );
		}

		if ( player_flags_scoped && pl->scoped ( ) )
			m_flags.push_back ( { _ ( "ZOOM" ), { 79, 132, 255 } } );

		for ( int i = 0; i < m_flags.size ( ); i++ ) {
			auto col = m_flags [ i ].second;

			render.string ( fonts [ fonts_t::font_esp_small ].m_data, box.x + box.w + 2, box.y + ( i * fonts [ fonts_t::font_esp_small ].m_height ), { col.r, col.g, col.b, 200 }, m_flags [ i ].first );
		}
	}
}

bool visuals_t::get_player_box ( player_t *pl, box_t &box ) {
	auto origin = pl->abs_origin ( );

	vec_t flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	auto min = pl->mins ( ) + origin;
	auto max = pl->maxs ( ) + origin;

	vec_t points [ ] = {
		vec_t ( min.x, min.y, min.z ),
		vec_t ( min.x, min.y, max.z ),
		vec_t ( min.x, max.y, min.z ),
		vec_t ( min.x, max.y, max.z ),
		vec_t ( max.x, min.y, min.z ),
		vec_t ( max.x, min.y, max.z ),
		vec_t ( max.x, max.y, min.z ),
		vec_t ( max.x, max.y, max.z ),
	};

	if ( !render.world_to_screen ( points [ 3 ], flb )
		|| !render.world_to_screen ( points [ 5 ], brt )
		|| !render.world_to_screen ( points [ 0 ], blb )
		|| !render.world_to_screen ( points [ 4 ], frt )
		|| !render.world_to_screen ( points [ 2 ], frb )
		|| !render.world_to_screen ( points [ 1 ], brb )
		|| !render.world_to_screen ( points [ 6 ], blt )
		|| !render.world_to_screen ( points [ 7 ], flt ) )
		return false;

	vec_t arr [ ] = { flb, brt, blb, frt, frb, brb, blt, flt };

	left = flb.x;
	top = flb.y;
	right = flb.x;
	bottom = flb.y;

	for ( auto i = 1; i < 8; i++ ) {
		if ( left > arr [ i ].x )
			left = arr [ i ].x;

		if ( bottom < arr [ i ].y )
			bottom = arr [ i ].y;

		if ( right < arr [ i ].x )
			right = arr [ i ].x;

		if ( top > arr [ i ].y )
			top = arr [ i ].y;
	}

	box.x = static_cast < int >( left );
	box.y = static_cast < int >( top );
	box.w = static_cast < int >( right - left );
	box.h = static_cast < int >( bottom - top );

	return true;
}