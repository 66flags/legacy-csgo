#include "includes.h"

void fix_angles ( vec_t &ang ) {
	interfaces::m_engine->get_view_angles ( ang );
	math::clamp ( ang );
	interfaces::m_engine->set_view_angles ( ang );
}

bool  __fastcall hooks_t::create_move ( void *ecx, void *edx, float input_sample_time, ucmd_t *ucmd ) {
	if ( !ucmd || !ucmd->m_command_number )
		return hooks.m_create_move.get_old_method < decltype ( &create_move ) > ( )( ecx, edx, input_sample_time, ucmd );

	g.m_cmd = ucmd;

	vec_t old_angs;
	interfaces::m_engine->get_view_angles ( old_angs );

	/* run movement stuff before prediction. */
	movement.pre_prediction ( ucmd );
	movement.run ( ucmd, old_angs );

	/* run prediction code. */
	prediction.predict ( ucmd, [ & ] {
	} );

	vec_t angs;
	fix_angles ( angs );

	/* correct player movement. */
	movement.correct_movement ( ucmd, angs );
}
