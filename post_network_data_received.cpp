#include "includes.h"

bool __fastcall hooks_t::post_network_data_received ( void *ecx, void *edx, int commands_acknowledged ) {
	auto entity = reinterpret_cast < player_t * > ( ecx );

	const auto ret = hooks.m_post_network_data_received.get_old_method < decltype ( &post_network_data_received ) > ( ) ( ecx, edx, commands_acknowledged );

	/* should we check for errors? */
	if ( g.m_local != entity || commands_acknowledged <= 0 )
		return ret;

	/* store predicted slot. */
	//prediction.m_start_data = entity->get_predicted_frame ( commands_acknowledged - 1 );

	/* call original. */
	return ret;
}