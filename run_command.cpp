#include "includes.h"

void __fastcall hooks_t::run_command ( void *ecx, void *edx, player_t *player, ucmd_t *ucmd, c_move_helper *move_helper ) {
	const auto ret = hooks.m_run_command.get_old_method < decltype ( &run_command ) > ( );

	if ( !g.m_local || !g.m_local->alive ( ) || player != g.m_local )
		return ret ( ecx, edx, player, ucmd, move_helper );

	//static int old_num = 0;

	//auto nci = interfaces::m_engine->get_net_channel_info ( );

	//if ( !interfaces::m_client_state->m_choked_commands ) {
	//	if ( nci && g.m_cmd->m_command_number > old_num ) {
	//		const auto latency = math::time_to_ticks ( nci->get_latency ( 1 ) );

	//		if ( prediction.m_vel_modifier < 1.0f )
	//			prediction.m_vel_modifier = std::clamp < float > ( prediction.m_vel_modifier + ( math::ticks_to_time ( 1 ) + latency ) * ( 1.0f / 2.5f ), 0.0f, 1.0f );

	//		old_num = g.m_cmd->m_command_number;
	//	}

	//	if ( !( g.m_local->flags ( ) & flags_t::on_ground ) )
	//		g.m_local->velocity_modifier ( ) = prediction.m_vel_modifier;
	//}

	ret ( ecx, edx, player, ucmd, move_helper );

	/* set move helper interface. */
	interfaces::m_move_helper = move_helper;
}