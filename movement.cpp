#include "includes.h"

movement_t movement { };

void movement_t::pre_prediction ( ucmd_t *ucmd ) {
	if ( g.m_local->movetype ( ) == movetypes_t::ladder || g.m_local->movetype ( ) == movetypes_t::noclip )
		return;

	if ( settings.get_item < bool > ( _ ( "misc.bunny_hop" ) ) ) {
		bool on_ground = g.m_local->flags ( ) & flags_t::on_ground;

		if ( ucmd->m_buttons & buttons_t::jump && !on_ground )
			ucmd->m_buttons &= ~buttons_t::jump;
	}
}

void movement_t::run ( ucmd_t *ucmd, const vec_t &old_angs ) {
	if ( !interfaces::m_engine->is_in_game ( ) || !interfaces::m_engine->is_connected ( ) )
		return;

	if ( !g.m_local || !g.m_local->alive ( ) )
		return;

	if ( g.m_local->movetype ( ) == movetypes_t::ladder || g.m_local->movetype ( ) == movetypes_t::noclip )
		return;

	static cvar_t *cl_sidespeed = interfaces::m_cvar->find_var ( HASH ( "cl_sidespeed" ) );
	static cvar_t *cl_forwardspeed = interfaces::m_cvar->find_var ( HASH ( "cl_forwardspeed" ) );

	if ( settings.get_item < bool > ( _ ( "misc.unlimited_duck" ) ) )
		ucmd->m_buttons |= buttons_t::bullrush;

	if ( settings.get_item < bool > ( _ ( "misc.auto_strafe" ) ) && g.m_local->vel ( ).length_2d ( ) >= 30.0f ) {
		if ( settings.get_item < int > ( _ ( "misc.auto_strafe_type" ) ) == 1 ) {
			directional_strafe ( ucmd, old_angs );
		}
		else {
			if ( !( g.m_local->flags ( ) & flags_t::on_ground ) ) {
				if ( std::abs ( ucmd->m_mousedx ) > 2 ) {
					ucmd->m_smove = ucmd->m_mousedx < 0 ? -cl_sidespeed->get_float ( ) : cl_sidespeed->get_float ( );
				}
				else {
					ucmd->m_smove = ucmd->m_command_number % 2 ? -cl_sidespeed->get_float ( ) : cl_sidespeed->get_float ( );
					ucmd->m_fmove = cl_forwardspeed->get_float ( );
				}
			}
		}
	}
}

void movement_t::directional_strafe ( ucmd_t *ucmd, const vec_t &old_angs ) {
	static int strafer_flags = 0;

	if ( !!( g.m_local->flags ( ) & flags_t::on_ground ) ) {
		strafer_flags = 0;
		return;
	}

	auto velocity = g.m_local->vel ( );
	auto velocity_len = velocity.length_2d ( );

	if ( velocity_len <= 0.0f ) {
		strafer_flags = 0;
		return;
	}

	auto ideal_step = std::min < float > ( 90.0f, 845.5f / velocity_len );
	auto velocity_yaw = ( velocity.y || velocity.x ) ? math::rad2deg ( atan2f ( velocity.y, velocity.x ) ) : 0.0f;

	auto unmod_angles = old_angs;
	auto angles = old_angs;

	if ( velocity_len < 2.0f && !!( ucmd->m_buttons & buttons_t::jump ) )
		ucmd->m_fmove = 450.0f;

	auto forward_move = ucmd->m_fmove;
	auto onground = !!( g.m_local->flags ( ) & flags_t::on_ground );

	if ( forward_move || ucmd->m_smove ) {
		ucmd->m_fmove = 0.0f;

		if ( velocity_len != 0.0f && abs ( velocity.z ) != 0.0f ) {
			if ( !onground ) {
			DO_IT_AGAIN:
				auto fwd = math::angle_vector ( angles );
				auto right = fwd.cross ( vec_t ( 0.0f, 0.0f, 1.0f ) );

				auto v262 = ( fwd.x * forward_move ) + ( ucmd->m_smove * right.x );
				auto v263 = ( right.y * ucmd->m_smove ) + ( fwd.y * forward_move );

				angles.y = ( v262 || v263 ) ? math::rad2deg ( atan2f ( v263, v262 ) ) : 0.0f;
			}
		}
	}

	auto yaw_to_use = 0.0f;

	strafer_flags &= ~4;

	if ( !onground ) {
		auto clamped_angles = angles.y;

		if ( clamped_angles < -180.0f ) clamped_angles += 360.0f;
		if ( clamped_angles > 180.0f ) clamped_angles -= 360.0f;

		yaw_to_use = old_angs.y;

		strafer_flags |= 4;
	}

	if ( strafer_flags & 4 ) {
		auto diff = angles.y - yaw_to_use;

		if ( diff < -180.0f ) diff += 360.0f;
		if ( diff > 180.0f ) diff -= 360.0f;

		if ( abs ( diff ) > ideal_step && abs ( diff ) <= 30.0f ) {
			auto move = 450.0f;

			if ( diff < 0.0f )
				move *= -1.0f;

			ucmd->m_smove = move;
			return;
		}
	}

	auto diff = angles.y - velocity_yaw;

	if ( diff < -180.0f ) diff += 360.0f;
	if ( diff > 180.0f ) diff -= 360.0f;

	auto step = 0.6f * ( ideal_step + ideal_step );
	auto side_move = 0.0f;

	if ( abs ( diff ) > 170.0f && velocity_len > 80.0f || diff > step && velocity_len > 80.0f ) {
		angles.y = step + velocity_yaw;
		ucmd->m_smove = -450.0f;
	}
	else if ( -step <= diff || velocity_len <= 80.0f ) {
		if ( strafer_flags & 1 ) {
			angles.y -= ideal_step;
			ucmd->m_smove = -450.0f;
		}
		else {
			angles.y += ideal_step;
			ucmd->m_smove = 450.0f;
		}
	}
	else {
		angles.y = velocity_yaw - step;
		ucmd->m_smove = 450.0f;
	}

	if ( !( ucmd->m_buttons & buttons_t::back ) && !ucmd->m_smove )
		goto DO_IT_AGAIN;

	strafer_flags ^= ( strafer_flags ^ ~strafer_flags ) & 1;

	if ( angles.y < -180.0f ) angles.y += 360.0f;
	if ( angles.y > 180.0f ) angles.y -= 360.0f;

	correct_movement ( ucmd, angles );
}

void movement_t::correct_movement ( ucmd_t *ucmd, const vec_t& wish_dir ) {
	static cvar_t *cl_sidespeed = interfaces::m_cvar->find_var ( HASH ( "cl_sidespeed" ) );
	static cvar_t *cl_forwardspeed = interfaces::m_cvar->find_var ( HASH ( "cl_forwardspeed" ) );
	static cvar_t *cl_upspeed = interfaces::m_cvar->find_var ( HASH ( "cl_upspeed" ) );

	vec_t view_fwd, view_right, view_up, cmd_fwd, cmd_right, cmd_up;
	auto view_angles = ucmd->m_ang;

	math::angle_vectors ( wish_dir, &view_fwd, &view_right, &view_up );
	math::angle_vectors ( view_angles, &cmd_fwd, &cmd_right, &cmd_up );

	const float v8 = view_fwd.length_2d ( );
	const float v10 = view_right.length_2d ( );
	const float v12 = sqrt ( view_up.z * view_up.z );

	const vec_t norm_view_fwd ( ( 1.0f / v8 ) * view_fwd.x, ( 1.0f / v8 ) * view_fwd.y, 0.0f );
	const vec_t norm_view_right ( ( 1.0f / v10 ) * view_right.x, ( 1.0f / v10 ) * view_right.y, 0.0f );
	const vec_t norm_view_up ( 0.0f, 0.0f, ( 1.0f / v12 ) * view_up.z );

	const float v14 = cmd_fwd.length_2d ( );
	const float v16 = cmd_right.length_2d ( );
	const float v18 = sqrt ( cmd_up.z * cmd_up.z );

	const vec_t norm_cmd_fwd ( ( 1.0f / v14 ) * cmd_fwd.x, ( 1.0f / v14 ) * cmd_fwd.y, 0.0f );
	const vec_t norm_cmd_right ( ( 1.0f / v16 ) * cmd_right.x, ( 1.0f / v16 ) * cmd_right.y, 0.0f );
	const vec_t norm_cmd_up ( 0.0f, 0.0f, ( 1.0f / v18 ) * cmd_up.z );

	const float v22 = norm_view_fwd.x * ucmd->m_fmove;
	const float v26 = norm_view_fwd.y * ucmd->m_fmove;
	const float v28 = norm_view_fwd.z * ucmd->m_fmove;
	const float v24 = norm_view_right.x * ucmd->m_smove;
	const float v23 = norm_view_right.y * ucmd->m_smove;
	const float v25 = norm_view_right.z * ucmd->m_smove;
	const float v30 = norm_view_up.x * ucmd->m_umove;
	const float v27 = norm_view_up.z * ucmd->m_umove;
	const float v29 = norm_view_up.y * ucmd->m_umove;

	ucmd->m_fmove = ( ( ( ( norm_cmd_fwd.x * v24 ) + ( norm_cmd_fwd.y * v23 ) ) + ( norm_cmd_fwd.z * v25 ) )
		+ ( ( ( norm_cmd_fwd.x * v22 ) + ( norm_cmd_fwd.y * v26 ) ) + ( norm_cmd_fwd.z * v28 ) ) )
		+ ( ( ( norm_cmd_fwd.y * v30 ) + ( norm_cmd_fwd.x * v29 ) ) + ( norm_cmd_fwd.z * v27 ) );
	ucmd->m_smove = ( ( ( ( norm_cmd_right.x * v24 ) + ( norm_cmd_right.y * v23 ) ) + ( norm_cmd_right.z * v25 ) )
		+ ( ( ( norm_cmd_right.x * v22 ) + ( norm_cmd_right.y * v26 ) ) + ( norm_cmd_right.z * v28 ) ) )
		+ ( ( ( norm_cmd_right.x * v29 ) + ( norm_cmd_right.y * v30 ) ) + ( norm_cmd_right.z * v27 ) );
	ucmd->m_umove = ( ( ( ( norm_cmd_up.x * v23 ) + ( norm_cmd_up.y * v24 ) ) + ( norm_cmd_up.z * v25 ) )
		+ ( ( ( norm_cmd_up.x * v26 ) + ( norm_cmd_up.y * v22 ) ) + ( norm_cmd_up.z * v28 ) ) )
		+ ( ( ( norm_cmd_up.x * v30 ) + ( norm_cmd_up.y * v29 ) ) + ( norm_cmd_up.z * v27 ) );

	ucmd->m_fmove = std::clamp ( ucmd->m_fmove, -cl_forwardspeed->get_float ( ), cl_forwardspeed->get_float ( ) );
	ucmd->m_smove = std::clamp ( ucmd->m_smove, -cl_sidespeed->get_float ( ), cl_sidespeed->get_float ( ) );
	ucmd->m_umove = std::clamp ( ucmd->m_umove, -cl_upspeed->get_float ( ), cl_upspeed->get_float ( ) );
}