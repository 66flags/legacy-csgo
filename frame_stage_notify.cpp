#include "includes.h"

void __fastcall hooks_t::frame_stage_notify ( void *ecx, void *edx, frame_stage_t stage ) {
	if ( stage != frame_stage_t::start )
		g.m_stage = stage;

	g.m_local = ( interfaces::m_engine->is_connected ( ) || interfaces::m_engine->is_in_game ( ) ) ? interfaces::m_entlist->get< player_t * > ( interfaces::m_engine->get_local_player ( ) ) : nullptr;

	if ( g.m_local && g.m_local->alive ( ) && stage == frame_stage_t::net_update_postdataupdate_end ) {
		prediction.m_old_vel_modifier = g.m_local->velocity_modifier ( );

		if ( g.m_local->velocity_modifier ( ) < prediction.m_old_vel_modifier ) {
			prediction.m_vel_modifier = g.m_local->velocity_modifier ( );
			prediction.force_update ( true ); // force game prediction to update new values
#if 0
			notify.push_log ( tfm::format ( _ ( "fixed vel mod: %f, received: %f, current %f\n" ), prediction.m_vel_modifier, prediction.m_old_vel_modifier, g.m_local->velocity_modifier ( ) ), { 255, 0, 255 } );
#endif
		}
	}

	hooks.m_frame_stage_notify.get_old_method < decltype ( &frame_stage_notify ) > ( )( ecx, edx, stage );
}