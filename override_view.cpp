#include "includes.h"

void __fastcall hooks_t::override_view ( void *ecx, void *edx, view_setup_t *setup ) {
	float dist = settings.get_item < int > ( _ ( "visuals.thirdperson_distance" ) );

	if ( !g.m_local || !interfaces::m_engine->is_in_game ( ) || !interfaces::m_engine->is_connected ( ) )
		return hooks.m_override_view.get_old_method < decltype ( &override_view ) > ( ) ( ecx, edx, setup );

	/* the size of the bounding hull used for collision checking. */
	constexpr float cam_hull_offset { 16.0f };

	if ( !g.m_local->alive ( ) ) {
		interfaces::m_input->cam_to_thirdperson ( );
		interfaces::m_input->m_camera_in_thirdperson = true;
		g.m_thirdperson = true;
		
		return hooks.m_override_view.get_old_method < decltype ( &override_view ) > ( ) ( ecx, edx, setup );
	}

	if ( settings.get_item < bool > ( _ ( "visuals.thirdperson" ) ) 
		&& input::keybind_active ( _ ( "Thirdperson" ), settings.get_item < int > ( _ ( "visuals.thirdperson_key" ) ), settings.get_item < int > ( _ ( "visuals.thirdperson_key_toggle" ) ) ) ) {
		interfaces::m_input->cam_to_thirdperson ( );

		const vec_t cam_hull_min ( -cam_hull_offset, -cam_hull_offset, -cam_hull_offset );
		const vec_t cam_hull_max ( cam_hull_offset, cam_hull_offset, cam_hull_offset );

		vec_t angles;
		interfaces::m_engine->get_view_angles ( angles );

		vec_t fwd, origin = g.m_local->shoot_pos ( );

		math::angle_vectors ( angles, &fwd );
		angles.z = dist;

		trace_filter_simple_game_t filter;
		filter.set_pass_entity ( g.m_local );

		trace_t tr;
		ray_t ray;
		ray.init ( origin, origin - ( fwd * angles.z ), cam_hull_min, cam_hull_max );

		interfaces::m_trace->trace_ray ( ray, mask_npcworldstatic, reinterpret_cast < trace_filter_t* > ( &filter ), &tr );
		angles.z *= tr.m_fraction;

		interfaces::m_input->m_camera_offset = angles;
		interfaces::m_input->m_camera_in_thirdperson = true;
		g.m_thirdperson = true;
	}
	else {
		interfaces::m_input->cam_to_firstperson ( );
		interfaces::m_input->m_camera_in_thirdperson = false;
		g.m_thirdperson = false;
	}

	hooks.m_override_view.get_old_method < decltype ( &override_view ) > ( ) ( ecx, edx, setup );
}