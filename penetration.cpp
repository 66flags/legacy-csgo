#include "includes.h"

penetration_t penetration { };

enum {
	char_tex_antlion = 'A',
	char_tex_bloodyflesh = 'B',
	char_tex_concrete = 'C',
	char_tex_dirt = 'D',
	char_tex_eggshell = 'E',
	char_tex_flesh = 'F',
	char_tex_grate = 'G',
	char_tex_alienflesh = 'H',
	char_tex_clip = 'I',
	char_tex_grass = 'J',
	char_tex_snow = 'K',
	char_tex_plastic = 'L',
	char_tex_metal = 'M',
	char_tex_sand = 'N',
	char_tex_foliage = 'O',
	char_tex_computer = 'P',
	char_tex_asphalt = 'Q',
	char_tex_brick = 'R',
	char_tex_slosh = 'S',
	char_tex_tile = 'T',
	char_tex_cardboard = 'U',
	char_tex_vent = 'V',
	char_tex_wood = 'W',
	char_tex_glass = 'Y',
	char_tex_warpshield = 'Z'
};

fire_data_t penetration_t::run ( const vec_t src, const vec_t end, player_t *ent, bool is_zeus, weapon_info_t *info_override ) {
	fire_data_t info { };

	if ( !ent )
		return { };

	const auto weapon = g.m_local->weapon ( );

	if ( !weapon )
		return { };

	const auto data = info_override == nullptr ? weapon->data ( ) : info_override;

	if ( !data )
		return { };

	info.m_damage = data->m_damage;

	trace_filter_t filter { };

	filter.m_skip = g.m_local;

	bool result = simulate_fire_bullet ( data, src, end, &filter, info, is_zeus, ent );

	if ( result && info.m_damage < 1.f )
		return { };

	return info;
}

bool penetration_t::simulate_fire_bullet ( const weapon_info_t *data, vec_t src, vec_t pos, trace_filter_t *filter, fire_data_t &fire_info, bool is_zeus, player_t *ent ) {
	static cvar_t *sv_penetration_type = interfaces::m_cvar->find_var ( HASH ( "sv_penetration_type" ) );

	fire_info.m_penetrate_count = 4;

	float penetration = data->m_penetration;
	float current_distance = 0.0f, damage_modifier = 0.5f, penetration_power = 35.0f, penetration_modifier = 1.0f;
	float weapon_range = data->m_range;

	vec_t dir = ( pos - src ).normalized ( );

	if ( sv_penetration_type->get_int ( ) == 1 )
		penetration_power = penetration;

	int penetration_count = fire_info.m_penetrate_count;

	while ( fire_info.m_penetrate_count > 0 && fire_info.m_damage > 1.0f ) {
		vec_t end = src + ( dir * ( weapon_range - current_distance ) );

		trace_t enter_trace;
		ray_t ray;
		
		ray.init ( src, end );

		interfaces::m_trace->trace_ray ( ray, mask_shot_hull | contents_hitbox, filter, &enter_trace );
		clip_trace_to_player ( ent, src, end + ( dir * 40.f ), mask_shot_hull | contents_hitbox, filter, &enter_trace );
		
		if ( enter_trace.m_fraction == 1.0f )
			break;

		auto surface_data = interfaces::m_phys_props->get_surface_data ( enter_trace.m_surface.m_props );

		current_distance += enter_trace.m_fraction * ( weapon_range - current_distance );
		fire_info.m_damage *= std::powf ( data->m_range_modifier, ( current_distance / 500.f ) );
		
		int enter_material = surface_data->m_game.m_material;
		
		penetration_modifier = surface_data->m_game.m_penetration_modifier;
		damage_modifier = surface_data->m_game.m_damage_modifier;
		penetration = data->m_penetration;

		if ( current_distance > 3000.f || penetration_modifier < 0.1f )
			break;

		if ( enter_trace.m_hit_entity && enter_trace.m_hit_entity == ent && ( enter_trace.m_hitgroup >= 0 && enter_trace.m_hitgroup <= 7 ) ) {
			fire_info.m_out_damage = scale_dmg ( ent, fire_info.m_damage, data->m_armor_ratio, enter_trace.m_hitgroup, is_zeus ); /* return our damage if our ray is visible. */
			fire_info.m_damage = fire_info.m_out_damage;
			
			return true;
		}

		bool hit_grate = ( enter_trace.m_contents & contents_t::contents_grate ) != 0;

		bool bullet_stopped = handle_bullet_penetration ( 
			ent, 
			penetration, 
			enter_material,
			hit_grate,
			enter_trace,
			dir, 
			surface_data,
			penetration_modifier,
			damage_modifier, 
			penetration_power, 
			penetration_count,
			src, 
			weapon_range, 
			fire_info.m_trace_length,
			fire_info.m_damage );

		if ( !bullet_stopped )
			break;
	}

	return true;
}

bool penetration_t::is_breakable_ent ( player_t *ent ) {
	static auto _is_breakable = pattern::find ( _ ( "client.dll" ), _ ( "E8 ? ? ? ? 84 C0 75 A1" ) ).rel32 ( );

	static auto m_takedamage_offset = _is_breakable.add ( 38 ).deref ( ).as< std::uint32_t > ( );

	if ( !ent || !ent->idx ( ) )
		return false;

	auto &takedmg = *reinterpret_cast< std::uint8_t * > ( reinterpret_cast< std::uintptr_t >( ent ) + m_takedamage_offset );

	const auto backup_takedmg = takedmg;
	takedmg = 2;

	const auto ret = _is_breakable.as< bool ( __thiscall * )( entity_t * ) > ( ) ( ent );

	takedmg = backup_takedmg;

	return ret;
}

bool penetration_t::trace_to_exit ( vec_t start, vec_t dir, vec_t &end, trace_t *trace_enter, trace_t *trace_exit, float step_size, float max_distance ) {
	static cvar_t *sv_clip_penetration_traces_to_players = interfaces::m_cvar->find_var ( HASH ( "sv_clip_penetration_traces_to_players" ) );

	float dist = 0.f;
	vec_t last = start;
	int start_contents = 0;

	while ( dist <= max_distance ) {
		dist += step_size;

		end = start + ( dir * dist );

		vec_t trace_end = end - ( dir * step_size );

		start_contents = interfaces::m_trace->get_point_contents ( end, 0x4600400B, nullptr );

		int current_contents = interfaces::m_trace->get_point_contents ( end, 0x4600400B );

		if ( ( current_contents & 0x600400B ) == 0 || ( ( current_contents & 0x40000000 ) && start_contents != current_contents ) ) {
			ray_t ray_world { };

			ray_world.init ( end, trace_end );
			interfaces::m_trace->trace_ray ( ray_world, 0x600400B, nullptr, trace_exit );
			
			if ( sv_clip_penetration_traces_to_players->get_int ( ) )
				clip_trace_to_players ( end, trace_end, 0x4600400B, nullptr, trace_exit );

			if ( trace_exit->m_startsolid && ( trace_exit->m_surface.m_flags & surf_t::surf_hitbox ) ) {
				ray_t ray { };
				trace_filter_t filter { };

				filter.m_skip = trace_exit->m_hit_entity;
				ray.init ( end, start );

				interfaces::m_trace->trace_ray ( ray, 0x600400B, &filter, trace_enter );

				if ( trace_exit->did_hit ( ) && !trace_exit->m_startsolid ) {
					end = trace_exit->m_endpos;

					return true;
				}
			}
			
			if ( trace_exit->did_hit ( ) && !trace_exit->m_startsolid ) {
				bool start_is_no_draw = !!( trace_enter->m_surface.m_flags & surf_t::surf_nodraw );
				bool exit_is_no_draw = !!( trace_exit->m_surface.m_flags & surf_t::surf_nodraw );

				if ( exit_is_no_draw && is_breakable_ent ( static_cast< player_t * >( trace_enter->m_hit_entity ) ) && is_breakable_ent ( static_cast< player_t * >( trace_enter->m_hit_entity ) ) ) {
					end = trace_exit->m_endpos;
					return true;
				}
				
				if ( exit_is_no_draw == false || ( start_is_no_draw && exit_is_no_draw ) ) {
					vec_t normal = trace_exit->m_plane.m_normal;
					float dot_val = dir.dot ( normal );

					if ( dot_val <= 1.0f ) {
						end = end - ( dir * ( step_size * trace_exit->m_fraction ) );

						return true;
					}
				}
			}
			
			if ( trace_enter->did_hit_non_world_ent ( ) && is_breakable_ent ( static_cast< player_t * >( trace_enter->m_hit_entity ) ) ) {
				trace_exit = trace_enter;
				trace_exit->m_endpos = start + dir;

				return true;
			}
		}
	}

	return false;
}

bool penetration_t::handle_bullet_penetration ( player_t *ent, float &penetration, int &enter_material, bool &hit_grate, trace_t &tr, vec_t &direction, surfacedata_t *surface_data, float penetration_modifier, float damage_modifier, float penetration_power, int &penetration_count, vec_t &src, float distance, float current_distance, float &current_damage ) {
	static cvar_t *sv_penetration_type = interfaces::m_cvar->find_var ( HASH ( "sv_penetration_type" ) );
	static cvar_t *ff_damage_reduction_bullets = interfaces::m_cvar->find_var ( HASH ( "ff_damage_reduction_bullets" ) );
	static cvar_t *ff_damage_bullet_penetration = interfaces::m_cvar->find_var ( HASH ( "ff_damage_bullet_penetration" ) );

	bool is_no_draw = !!( tr.m_surface.m_flags & surf_t::surf_nodraw );
	bool failed_penetrate = false;

	if ( penetration_count == 0 && !hit_grate && !is_no_draw
		&& enter_material != char_tex_glass && enter_material != char_tex_grate )
		failed_penetrate = true;

	if ( penetration <= 0 || penetration_count <= 0 )
		failed_penetrate = true;

	vec_t penetration_end;
	trace_t exit_trace;

	if ( !trace_to_exit ( tr.m_endpos, direction, penetration_end, &tr, &exit_trace, 4.0f, 90.0f ) ) {
		if ( ( interfaces::m_trace->get_point_contents ( tr.m_endpos, mask_t::mask_shot_hull ) & mask_t::mask_shot_hull ) == 0 ) {
			failed_penetrate = true;
		}
	}

	if ( failed_penetrate == true )
		return true;

	surfacedata_t *exit_surface_data = interfaces::m_phys_props->get_surface_data ( exit_trace.m_surface.m_props );

	int exit_material = exit_surface_data->m_game.m_material;

	if ( sv_penetration_type->get_int ( ) == 1 ) {
		float damage_lost_percentage = 0.16f;

		if ( hit_grate || is_no_draw || enter_material == char_tex_glass || enter_material == char_tex_grate ) {
			if ( enter_material == char_tex_glass || enter_material == char_tex_grate ) {
				penetration_modifier = 3.0f;
				damage_lost_percentage = 0.05f;
			}
			else
				penetration_modifier = 1.0f;

			damage_modifier = 0.99f;
		}
		else if ( enter_material == char_tex_flesh && ff_damage_reduction_bullets->get_float ( ) == 0 && ( ent && tr.m_hit_entity && static_cast< player_t * >( tr.m_hit_entity )->is_player ( ) && tr.m_hit_entity->team ( ) == ent->team ( ) ) ) {
			if ( ff_damage_bullet_penetration->get_float ( ) == 0 ) {
				penetration_modifier = 0.f;
				return true;
			}

			penetration_modifier = ff_damage_bullet_penetration->get_float ( );
			damage_modifier = ff_damage_bullet_penetration->get_float ( );
		}
		else {
			float exit_penetration_modifier = exit_surface_data->m_game.m_penetration_modifier;
			float exit_damage_modifier = exit_surface_data->m_game.m_damage_modifier;

			penetration_modifier = ( penetration_modifier + exit_penetration_modifier ) / 2;
			damage_modifier = ( damage_modifier + exit_damage_modifier ) / 2;
		}

		if ( enter_material == exit_material ) {
			if ( exit_material == char_tex_wood || exit_material == char_tex_cardboard ) {
				penetration_modifier = 3.f;
			}
			else if ( exit_material == char_tex_plastic ) {
				penetration_modifier = 2.f;
			}
		}

		float trace_distance = ( exit_trace.m_endpos - tr.m_endpos ).length ( );
		float penetration_mod = std::fmaxf ( 0.f, ( 1.f / penetration_modifier ) );
		float percent_damage_chunk = current_damage * damage_lost_percentage;
		float pen_wep_mod = percent_damage_chunk + std::fmaxf ( 0.f, ( 3.f / penetration_power ) * 1.25f ) * ( penetration_mod * 3.0f );
		float lost_damage_object = ( ( penetration_mod * ( trace_distance * trace_distance ) ) / 24.f );
		float total_lost_damage = pen_wep_mod + lost_damage_object;

		current_damage -= std::fmaxf ( 0.f, total_lost_damage );
		
		if ( current_damage < 1.f )
			return true;

		current_distance += trace_distance;
		src = exit_trace.m_endpos;
		distance = ( distance - current_distance ) * 0.5f;
		penetration_count--;

		return false;
	}
	else {
		if ( hit_grate || is_no_draw ) {
			penetration_modifier = 1.0f;
			damage_modifier = 0.99f;
		}
		else {
			float exit_penetration_modifier = exit_surface_data->m_game.m_penetration_modifier;
			float exit_damage_modifier = exit_surface_data->m_game.m_damage_modifier;

			if ( exit_penetration_modifier < penetration_modifier ) {
				penetration_modifier = exit_penetration_modifier;
			}
			if ( exit_damage_modifier < damage_modifier ) {
				damage_modifier = exit_damage_modifier;
			}
		}

		if ( enter_material == exit_material ) {
			if ( exit_material == char_tex_wood || exit_material == char_tex_metal )
				penetration_modifier *= 2;
		}

		float trace_distance = ( exit_trace.m_endpos - tr.m_endpos ).length ( );

		if ( trace_distance > ( penetration_power * penetration_modifier ) )
			return true;

		current_damage *= damage_modifier;

		penetration_power -= trace_distance / penetration_modifier;
		current_distance += trace_distance;

		src = exit_trace.m_endpos;
		distance = ( distance - current_distance ) * 0.5f;
		penetration_count--;

		return false;
	}
}

float penetration_t::scale_dmg ( player_t *player, float damage, float armor_ratio, int hitgroup, bool is_zeus ) {
	auto weapon = player->weapon ( );
	
	if ( !weapon )
		return 0.f;

	static cvar_t *mp_damage_scale_ct_body = interfaces::m_cvar->find_var ( HASH ( "mp_damage_scale_ct_body" ) );
	static cvar_t *mp_damage_scale_t_body = interfaces::m_cvar->find_var ( HASH ( "mp_damage_scale_t_body" ) );
	static cvar_t *mp_damage_scale_ct_head = interfaces::m_cvar->find_var ( HASH ( "mp_damage_scale_ct_head" ) );
	static cvar_t *mp_damage_scale_t_head = interfaces::m_cvar->find_var ( HASH ( "mp_damage_scale_t_head" ) );

	float scale_body_damage = ( player->team ( ) == 3 ) ? mp_damage_scale_ct_body->get_float ( ) : mp_damage_scale_t_body->get_float ( );
	float head_damage_scale = ( player->team ( ) == 3 ) ? mp_damage_scale_ct_head->get_float ( ) : mp_damage_scale_t_head->get_float ( );

	static auto is_armored = [ ] ( player_t *player, int hitgroup ) -> bool {
		if ( player->armor ( ) <= 0 )
			return false;

		switch ( hitgroup ) {
		case hitgroup_t::hitgroup_generic:
		case hitgroup_t::hitgroup_chest:
		case hitgroup_t::hitgroup_stomach:
		case hitgroup_t::hitgroup_leftarm:
		case hitgroup_t::hitgroup_rightarm:
			return true;
			break;
		case hitgroup_t::hitgroup_head:
			if ( player->helmet ( ) )
				return true;
			break;
		default:
			break;
		}

		return false;
	};

	const int armor = player->armor ( );

	if ( player->heavy_armor ( ) )
		head_damage_scale *= 0.5f;

	if ( !is_zeus )
		/* NOTE: we don't need to scale based on armor / helmet since zeus will always target generic hitgroup. */
		switch ( hitgroup ) {
		case hitgroup_t::hitgroup_head:
			damage *= weapon->data ( ) ? ( weapon->data ( )->m_headshot_multiplier * head_damage_scale ) : ( 4.0f * head_damage_scale );
			break;
		case hitgroup_t::hitgroup_chest:
			damage *= 1.0f * scale_body_damage;
			break;
		case hitgroup_t::hitgroup_stomach:
			damage *= 1.25f * scale_body_damage;
			break;
		case hitgroup_t::hitgroup_leftarm:
		case hitgroup_t::hitgroup_rightarm:
			damage *= 1.0f * scale_body_damage;
			break;
		case hitgroup_t::hitgroup_leftleg:
		case hitgroup_t::hitgroup_rightleg:
			damage *= 0.75f * scale_body_damage;
			break;
		default:
			break;
		}

	if ( is_armored ( player, hitgroup ) ) {
		float armor_bonus = 0.5f, armor_ratio = 0.5f, heavy_armor_bonus = 1.0f;
		bool has_heavy = player->heavy_armor ( );

		if ( has_heavy ) {
			armor_ratio *= 0.5f;
			armor_bonus = 0.33f;
			heavy_armor_bonus = 0.33f;
		}

		float damage_to_health = damage * armor_ratio;
		float damage_to_armor = ( damage - damage_to_health ) * ( armor_bonus * heavy_armor_bonus );

		if ( damage_to_armor > static_cast< float > ( armor ) )
			damage_to_health = damage - static_cast< float > ( armor ) / armor_bonus;

		damage = damage_to_health;
	}

	return damage;
}

void penetration_t::clip_trace_to_player ( player_t *player, const vec_t &start, const vec_t &end, unsigned int mask, trace_filter_t *filter, trace_t *tr ) {
	if ( !player || !player->alive ( ) || player->dormant ( ) )
		return;

	float smallest_fraction = tr->m_fraction;
	const float max_range = 60.0f;

	ray_t ray;
	trace_t player_trace;
	
	ray.init ( start, end );

	vec_t world_space_center = player->origin ( ) + ( ( player->mins ( ) + player->maxs ( ) ) * 0.5f );

	if ( filter && filter->should_hit_entity ( player, mask ) == false )
		return;

	float range = math::distance_to_ray ( world_space_center, start, end );

	if ( range < 0.0f || range > max_range )
		return;

	interfaces::m_trace->clip_ray_to_entity ( ray, mask | contents_t::contents_hitbox, player, &player_trace );

	if ( player_trace.m_fraction < smallest_fraction ) {
		*tr = player_trace;
		smallest_fraction = player_trace.m_fraction;
	}
}

void penetration_t::clip_trace_to_players ( const vec_t &start, const vec_t &end, unsigned int mask, trace_filter_t *filter, trace_t *tr ) {
	for ( int i = 1; i <= interfaces::m_globals->m_max_clients; i++ ) {
		auto player = interfaces::m_entlist->get < player_t* > ( i );
		
		if ( player )
			clip_trace_to_player ( player, start, end, mask, filter, tr );
	}
}