#include "includes.h"

datamap_t *player_t::get_pred_desc_map ( ) {
	using get_pred_desc_map_fn = datamap_t* ( __thiscall * )( void * );
	return util::get_method < get_pred_desc_map_fn > ( this, 17 )( this );
}

void player_t::set_sequence ( int sequence ) {
	using set_sequence_fn = void ( __thiscall * )( void *, int );
	util::get_method < set_sequence_fn > ( this, 213 )( this, sequence );
}

void player_t::modify_eye_position ( animstate_t *state, vec_t *input_eye_pos ) {
	const auto e = state->m_player;

	if ( !e )
		return;

	const auto ground_ent = interfaces::m_entlist->get_by_handle< player_t * > ( e->handle ( ) );

	if ( !state->m_landing || state->m_duck_amount == 0.0f || ground_ent )
		return;

	interfaces::m_mdl_cache->begin_lock ( );

	const auto head_bone = e->lookup_bone ( _ ( "head_0" ) );

	if ( head_bone != -1 ) {
		vec_t head_pos, temp;
		e->bone_pos ( head_bone, head_pos, temp );

		float v6 = head_pos.z + 1.7f;

		if ( input_eye_pos->z > v6 ) {
			float v8 = 0.0f;
			float v9 = ( float ) ( fabs ( input_eye_pos->z - v6 ) - 4.0f ) * 0.16666667f;

			if ( v9 >= 0.0 )
				v8 = fminf ( v9, 1.0f );

			input_eye_pos->z = ( ( v6 - input_eye_pos->z ) * ( ( ( v8 * v8 ) * 3.0f ) - ( ( ( v8 * v8 ) * 2.0f ) * v8 ) ) ) + input_eye_pos->z;
		}
	}

	interfaces::m_mdl_cache->end_lock ( );
}

bool player_t::is_enemy ( player_t *other ) {
	static cvar_t *mp_teammates_are_enemies = interfaces::m_cvar->find_var ( HASH ( "mp_teammates_are_enemies" ) );

	if ( !this || !other || this == other )
		return false;

	if ( interfaces::m_game_type->get_game_type ( ) == game_type_t::danger_zone ) {
		const auto my_team = survival_team ( );

		if ( my_team == -1 )
			return true;

		return my_team != other->survival_team ( );
	}
	else {
		const auto my_team = static_cast< int >( team ( ) ) - 2;
		auto v11 = false;

		if ( my_team ) {
			if ( my_team != 1 )
				return false;

			v11 = other->team ( ) == 2;
		}
		else
			v11 = other->team ( ) == 3;

		if ( v11 )
			return true;

		return mp_teammates_are_enemies->get_bool ( );
	}

	return false;
}

std::uint8_t *player_t::get_original_network_data_object ( ) const {
	return *reinterpret_cast < std::uint8_t ** > ( reinterpret_cast < std::uintptr_t > ( this ) + 0x92C );
}

std::uint8_t *player_t::get_predicted_frame ( int frame_num ) {
	if ( !get_original_network_data_object ( ) )
		return nullptr;

	return *reinterpret_cast< std::uint8_t ** > ( reinterpret_cast< std::uintptr_t > ( this ) + 4 * ( frame_num % 0x96 ) + 0x478 );
}

void player_t::select_item ( const char *name, int sub_type ) {
	using select_item_fn = void ( __thiscall * )( void *, const char *, int );
	util::get_method < select_item_fn > ( this, 319 )( this, name, sub_type );
}

void player_t::think ( ) {
	using think_fn = void ( __thiscall * )( void * );
	util::get_method < think_fn > ( this, 137 )( this );
}

void player_t::pre_think ( ) {
	using pre_think_fn = void ( __thiscall * )( void * );
	util::get_method < pre_think_fn > ( this, 307 )( this );
}

bool player_t::is_player ( ) {
	using is_player_fn = bool ( __thiscall * )( void * );
	return util::get_method < is_player_fn > ( this, 158 )( this );
}

vec_t player_t::shoot_pos ( ) {
	vec_t pos;
	util::get_method < void ( __thiscall * )( void *, const vec_t & ) > ( this, 163 )( this, pos );

	if ( use_new_animstate ( ) ) {
		auto state = animstate ( );

		if ( state )
			modify_eye_position ( state, &pos );
	}

	return pos;
}

void player_t::set_abs_origin ( const vec_t &abs_origin ) {
	using set_abs_origin_fn = void ( __thiscall * )( void *, const vec_t & );
	static auto o_set_abs_origin = pattern::find ( _ ( "client.dll" ), _ ( "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ? ? ? ? 8B 7D" ) ).as< set_abs_origin_fn > ( );
	o_set_abs_origin ( this, abs_origin );
}

void player_t::set_abs_angles ( const vec_t &abs_angles ) {
	using set_abs_angles_fn = void ( __thiscall * )( void *, const vec_t & );
	static auto o_set_abs_angles = pattern::find ( _ ( "client.dll" ), _ ( "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1" ) ).as< set_abs_angles_fn > ( );
	o_set_abs_angles ( this, abs_angles );
}

bool player_t::physics_run_think ( int think_method ) {
	using physics_run_think_fn = bool ( __thiscall * )( void *, int );
	static auto o_physics_run_think_fn = pattern::find ( _ ( "client.dll" ), _ ( "55 8B EC 83 EC 10 53 56 57 8B F9 8B 87 ? ? ? ? C1" ) ).as< physics_run_think_fn > ( );
	return o_physics_run_think_fn ( this, think_method );
}

void player_t::bone_pos ( int bone, vec_t &out, vec_t &q ) {
	using bone_pos_fn = void ( __thiscall * )( void *, int, vec_t &, vec_t & );
	static auto o_bone_pos = pattern::find ( _ ( "client.dll" ), _ ( "55 8B EC 83 E4 F8 56 8B F1 57 83" ) ).as< bone_pos_fn > ( );
	o_bone_pos ( this, bone, out, q );
}

int player_t::lookup_bone ( const char *name ) {
	using lookup_bone_fn = int ( __thiscall * )( void *, const char * );
	static auto o_lookup_bone = pattern::find ( _ ( "client.dll" ), _ ( "55 8B EC 53 56 8B F1 57 83 ? ? ? ? ? ? 75" ) ).as< lookup_bone_fn > ( );
	return o_lookup_bone ( this, name );
}

weapon_t *player_t::weapon ( ) {
	return interfaces::m_entlist->get_by_handle< weapon_t * > ( weapon_handle ( ) );
}