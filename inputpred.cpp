#include "includes.h"

inputpred_t prediction { };

void post_think ( player_t *ent ) {
	interfaces::m_mdl_cache->begin_lock ( );

	static auto post_think_vphysics = pattern::find ( _ ( "client.dll" ), _ ( "55 8B EC 83 E4 F8 81 EC ? ? ? ? 53 8B D9 56 57 83 BB" ) ).as< bool ( __thiscall * )( player_t * ) > ( );
	static auto simulate_player_simulated_entities = pattern::find ( _ ( "client.dll" ), _ ( "56 8B F1 57 8B BE ? ? ? ? 83 EF 01 78 72 90 8B 86" ) ).as< void ( __thiscall * )( void * ) > ( );

	if ( ent->alive ( ) ) {
		util::get_method < void ( __thiscall * )( void * ) > ( ent, 329 ) ( ent );

		if ( ent->flags ( ) & flags_t::on_ground )
			ent->fall_vel ( ) = 0.f;

		if ( ent->sequence ( ) == -1 )
			ent->set_sequence ( 0 );

		util::get_method < void ( __thiscall * )( void * ) > ( ent, 214 ) ( ent );
		post_think_vphysics ( ent );
	}
	
	simulate_player_simulated_entities ( ent );
	interfaces::m_mdl_cache->end_lock ( );
}

void inputpred_t::pre_create_move ( ) {
	//for ( int i = 0; i < 150; i++ ) {
	//	m_start_data = ( i == -1 ) ? g.m_local->get_original_network_data_object ( ) : g.m_local->get_predicted_frame ( i );

	//	c_prediction_copy copy_helper ( pc_everything, ( byte * ) m_start_data, true, ( const byte * ) g.m_local, false, transferdata_copyonly );
	//	copy_helper.transfer_data ( "inputpred_t::pre_create_move", g.m_local->idx ( ), g.m_local->get_pred_desc_map ( ) );
	//	m_end_data = m_start_data;
	//}
}

void inputpred_t::predict ( ucmd_t *ucmd, const std::function < void ( ) > &fn ) {
	prediction.begin ( ucmd );

	//c_prediction_copy copy_helper ( pc_everything, ( byte * ) m_end_data, true, ( const byte * ) g.m_local, false, transferdata_copyonly );
	//copy_helper.transfer_data ( "inputpred_t::predict", g.m_local->idx ( ), g.m_local->get_pred_desc_map ( ) );
	
	fn ( );
	prediction.end ( ucmd );
}

void inputpred_t::begin ( ucmd_t *ucmd ) {
	if ( !g.m_local || !ucmd || !interfaces::m_move_helper )
		return;
	
	if ( interfaces::m_client_state->m_delta_tick < 0 )
		prediction.force_update ( false );

	g.m_local->current_cmd ( ) = ucmd;
	g.m_local->last_cmd ( ) = *ucmd;

	if ( !m_prediction_player || !m_prediction_seed ) {
		m_prediction_seed = pattern::find ( _ ( "client.dll" ), _ ( "8B 47 40 A3" ) ).add ( 4 ).deref ( ).as< std::uintptr_t > ( );
		m_prediction_player = pattern::find ( _ ( "client.dll" ), _ ( "0F 5B C0 89 35" ) ).add ( 5 ).deref ( ).as< std::uintptr_t > ( );
	}

	*reinterpret_cast< int * >( m_prediction_seed ) = ucmd ? ucmd->m_random_seed : -1;
	*reinterpret_cast< player_t ** >( m_prediction_player ) = g.m_local;

	m_vel = g.m_local->vel ( );
	m_flags = g.m_local->flags ( );

	m_curtime = interfaces::m_globals->m_curtime;
	m_frametime = interfaces::m_globals->m_frametime;

	interfaces::m_globals->m_curtime = g.m_local->tick_base ( ) * interfaces::m_globals->m_interval_per_tick;
	interfaces::m_globals->m_frametime = interfaces::m_prediction->m_engine_paused ? 0 : interfaces::m_globals->m_interval_per_tick;

	m_is_first_time_predicted = interfaces::m_prediction->m_is_first_time_predicted;
	m_in_prediction = interfaces::m_prediction->m_in_prediction;

	interfaces::m_prediction->m_is_first_time_predicted = false;
	interfaces::m_prediction->m_in_prediction = true;

	ucmd->m_buttons |= g.m_local->button_forced ( );
//	ucmd->m_buttons &= ~g.m_local->button_disabled ( );

	interfaces::m_move_helper->set_host ( g.m_local );
	interfaces::m_game_movement->start_track_prediction_errors ( g.m_local );

	if ( ucmd->m_weaponselect != 0 ) {
		weapon_t *weapon = g.m_local->weapon ( );

		if ( weapon ) {
			auto data = weapon->data ( );

			if ( data )
				g.m_local->select_item ( data->m_weapon_name, ucmd->m_weaponsubtype );
		}
	}

	const auto buttons_changed = ucmd->m_buttons ^ g.m_local->button_last ( );

	g.m_local->buttons ( ) = g.m_local->button_last ( );
	g.m_local->button_last ( ) = ucmd->m_buttons;
	g.m_local->button_pressed ( ) = ucmd->m_buttons & buttons_changed;
	g.m_local->button_released ( ) = buttons_changed & ~ucmd->m_buttons;

	interfaces::m_prediction->check_moving_ground ( g.m_local, interfaces::m_globals->m_frametime );
	interfaces::m_prediction->set_local_view_angles ( ucmd->m_ang );

	if ( g.m_local->physics_run_think ( 0 ) )
		g.m_local->pre_think ( );

	const auto think_tick = g.m_local->think_tick ( );

	if ( think_tick > 0 && think_tick <= g.m_local->tick_base ( ) ) {
		g.m_local->think_tick ( ) = -1;
		static auto set_next_think = pattern::find ( _ ( "client.dll" ), _ ( "55 8B EC 56 57 8B F9 8B B7 ? ? ? ? 8B C6 C1 E8 16 24 01 74 18" ) ).as< void ( __thiscall * )( void *, int ) > ( );
		set_next_think ( g.m_local, 0 );
		g.m_local->think ( );
	}

	interfaces::m_prediction->setup_move ( g.m_local, ucmd, interfaces::m_move_helper, &data );
	interfaces::m_game_movement->process_movement ( g.m_local, &data );

	interfaces::m_prediction->finish_move ( g.m_local, ucmd, &data );
	interfaces::m_move_helper->process_impacts ( );

	post_think ( g.m_local );

	interfaces::m_prediction->m_is_first_time_predicted = m_is_first_time_predicted;
	interfaces::m_prediction->m_in_prediction = m_in_prediction;
}

void inputpred_t::end ( ucmd_t *ucmd ) {
	if ( !g.m_local || !ucmd || !interfaces::m_move_helper )
		return;

	interfaces::m_game_movement->finish_track_prediction_errors ( g.m_local );
	interfaces::m_move_helper->set_host ( nullptr );

	g.m_local->current_cmd ( ) = nullptr;

	*reinterpret_cast< int * >( m_prediction_seed ) = -1;
	*reinterpret_cast< player_t ** >( m_prediction_player ) = nullptr;

	interfaces::m_game_movement->reset ( );

	interfaces::m_globals->m_curtime = m_curtime;
	interfaces::m_globals->m_frametime = m_frametime;
}

void inputpred_t::restore_to_pre_prediction ( ) {
	c_prediction_copy copy_helper ( pc_everything, ( byte * ) g.m_local, true, ( const byte * ) m_start_data, false, transferdata_copyonly );		
	copy_helper.transfer_data ( "inputpred_t::restore_to_pre_prediction", g.m_local->idx ( ), g.m_local->get_pred_desc_map ( ) );
}

void inputpred_t::restore_to_post_prediction ( ) {
	c_prediction_copy copy_helper ( pc_everything, ( byte * ) g.m_local, true, ( const byte * ) m_end_data, false, transferdata_copyonly );
	copy_helper.transfer_data ( "inputpred_t::restore_to_post_prediction", g.m_local->idx ( ), g.m_local->get_pred_desc_map ( ) );
}

void inputpred_t::restore ( prediction_data_t *data, player_t *player ) {
	if ( !player || !g.m_local )
		return;

	player->tick_base ( ) = data->m_tick_base;
	player->flags ( ) = data->m_flags;
	player->movetype ( ) = data->m_movetype;
	player->handle ( ) = data->m_ground_entity;
	player->crouch_amount ( ) = data->m_crouch_amount;
	player->crouch_speed ( ) = data->m_crouch_speed;
	player->origin ( ) = data->m_origin;
	player->vel ( ) = data->m_velocity;
	player->base_vel ( ) = data->m_base_velocity;
	player->fall_vel ( ) = data->m_fall_velocity;
	player->velocity_modifier ( ) = data->m_velocity_modifier;
	player->view_offset ( ) = data->m_view_offset;
	player->aim_punch ( ) = data->m_aim_punch_angle;
	player->aim_punch_vel ( ) = data->m_aim_punch_angle_vel;
	player->view_punch ( ) = data->m_view_punch_angle;

	const auto weapon = player->weapon ( );

	if ( weapon ) {
		weapon->recoil_index ( ) = data->m_recoil_index;
		weapon->accuracy_penalty ( ) = data->m_accuracy_penalty;
	}
}

void inputpred_t::store ( prediction_data_t *data, player_t *player ) {
	if ( !player || !g.m_cmd )
		return;

	data->m_tick_base = player->tick_base ( );
	data->m_flags = player->flags ( );
	data->m_movetype = player->movetype ( );
	data->m_ground_entity = player->handle ( );
	data->m_crouch_amount = player->crouch_amount ( );
	data->m_crouch_speed = player->crouch_speed ( );
	data->m_origin = player->origin ( );
	data->m_velocity = player->vel ( );
	data->m_base_velocity = player->base_vel ( );
	data->m_fall_velocity = player->fall_vel ( );
	data->m_velocity_modifier = player->velocity_modifier ( );
	data->m_view_offset = player->view_offset ( );
	data->m_aim_punch_angle = player->aim_punch ( );
	data->m_aim_punch_angle_vel = player->aim_punch_vel ( );
	data->m_view_punch_angle = player->view_punch ( );

	const auto weapon = player->weapon ( );

	if ( weapon ) {
		data->m_recoil_index = weapon->recoil_index ( );
		data->m_accuracy_penalty = weapon->accuracy_penalty ( );
	}
}

void inputpred_t::fix_velocity_modifier ( ) {
	//if ( g.m_local->velocity_modifier ( ) < prediction.m_old_vel_modifier ) {
	//	auto nci = interfaces::m_engine->get_net_channel_info ( );

	//	if ( nci ) {
	//		const auto latency = math::time_to_ticks ( nci->get_latency ( 1 ) );

	//		if ( prediction.m_vel_modifier < 1.0f )
	//			prediction.m_vel_modifier = std::clamp < float > ( prediction.m_vel_modifier + ( math::ticks_to_time ( 1 ) + latency ) * ( 1.0f / 2.5f ), 0.0f, 1.0f );
	//	}

	//	prediction.m_vel_modifier = g.m_local->velocity_modifier ( );
	//	prediction.force_update ( true );
	//}
}

void inputpred_t::force_update ( bool error ) {
	if ( !g.m_local || !interfaces::m_prediction || !interfaces::m_move_helper )
		return;

	if ( error ) {
		interfaces::m_prediction->m_prev_ack_had_errors = 1;
		interfaces::m_prediction->m_cmds_predicted = 0;
	}
	
	if ( interfaces::m_client_state->m_delta_tick > 0 && error )
		m_has_error = false;

	interfaces::m_prediction->update (
		interfaces::m_client_state->m_delta_tick,
		interfaces::m_client_state->m_delta_tick > 0,
		interfaces::m_client_state->m_last_command_ack,
		interfaces::m_client_state->m_last_outgoing_command + interfaces::m_client_state->m_choked_commands );
}