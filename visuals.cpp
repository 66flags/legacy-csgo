#include "includes.h"

visuals_t visuals { };

void visuals_t::paint ( ) {
	for ( int i = 1; i <= interfaces::m_globals->m_max_clients; i++ ) {
		auto ent = interfaces::m_entlist->get < player_t* > ( i );

		if ( !ent || !ent->alive ( ) || ent->team ( ) == g.m_local->team ( ) || ent->dormant ( ) )
			continue;

		box_t box;

		if ( !get_box_bounds ( ent, box ) )
			continue;

		//render.outlined_rect ( box.x, box.y, box.w, box.h, { 255, 255, 255 } );
	}
}

bool visuals_t::get_box_bounds ( player_t *ent, box_t &box ) {
	auto origin = ent->origin ( );

	vec_t flb, brt, blb, frt, frb, brb, blt, flt;
	float left, top, right, bottom;

	auto min = ent->mins ( ) + origin;
	auto max = ent->maxs ( ) + origin;

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

	box.x = static_cast< int >( left );
	box.y = static_cast< int >( top );
	box.w = static_cast< int >( right - left );
	box.h = static_cast< int >( bottom - top );

	return true;
}