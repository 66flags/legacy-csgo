#include "includes.h"

prediction_error_handler_t prediction_error_handler { };

template < typename t >
bool check_for_error ( t predicted, t networked ) {
	return predicted != networked;
}

void prediction_error_handler_t::post_data_network_received ( player_t *entity, int commands_acknowledged ) {
	if ( !g.m_local )
		return;

	static cvar_t *cl_predict = interfaces::m_cvar->find_var ( HASH ( "cl_predict" ) );

	/* don't run this if we're not running prediction. */
	if ( !cl_predict->get_bool ( ) )
		return;

	/* get networked, predicted datamaps. */
	const auto networked_slot = entity->get_original_network_data_object ( );
	const auto predicted_slot = entity->get_predicted_frame ( commands_acknowledged - 1 );
}

void prediction_error_handler_t::handle ( const char *field_name ) {
	
}