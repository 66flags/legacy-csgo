#pragma once

class player_t;
class weapon_t;

struct animstate_t {
	const int *m_layer_order_preset;
	bool m_first_run_since_init;
	bool m_first_foot_plant_since_init;
	int	m_last_procedural_foot_plant_update_frame;
	float m_eye_position_smooth_lerp;
	float m_strafe_change_weight_smooth_fall_off;
	PAD ( 60 );
	int m_cached_model_idx;
	float m_step_height_left;
	float m_step_height_right;
	weapon_t *m_weapon_last_bone_setup;
	player_t *m_player;
	weapon_t *m_weapon;
	weapon_t *m_weapon_last;
	float m_last_update_time;
	int	m_last_update_frame;
	float m_last_update_increment;
	float m_eye_yaw;
	float m_eye_pitch;
	float m_abs_yaw;
	float m_abs_yaw_last;
	float m_move_yaw;
	float m_move_yaw_ideal;
	float m_move_yaw_current_to_ideal;
	float m_time_to_align_lower_body;
	float m_primary_cycle;
	float m_move_weight;
	float m_move_weight_smoothed;
	float m_duck_amount;
	float m_duck_additional;
	float m_recrouch_weight;
	vec_t m_origin;
	vec_t m_origin_last;
	vec_t m_vel;
	vec_t m_vel_normalized;
	vec_t m_vel_normalized_non_zero;
	float m_vel_length_2d;
	float m_vel_length_z;
	float m_run_speed_normalized;
	float m_walk_speed_normalized;
	float m_crouch_speed_normalized;
	float m_duration_moving;
	float m_duration_still;
	bool m_on_ground;
	bool m_landing;
	float m_jump_to_fall;
	float m_duration_in_air;
	float m_left_ground_height;
	float m_hit_ground_weight;
	float m_walk_to_run_transition;
	PAD ( 4 );
	float m_in_air_smooth_value;
	bool m_on_ladder;
	float m_ladder_weight;
	float m_ladder_speed;
	bool m_walk_to_run_transition_state;
	bool m_defuse_started;
	bool m_plant_anim_started;
	bool m_twitch_anim_started;
	bool m_adjust_started;
	PAD ( 14 );
	float m_next_twitch_time;
	float m_time_of_last_known_injury;
	float m_last_velocity_test_time;
	vec_t m_last_vel;
	vec_t m_target_accel;
	vec_t m_accel;
	float m_accel_weight;
	PAD ( 12 );
	float m_strafe_change_weight;
	float m_strafe_change_target_weight;
	float m_strafe_change_cycle;
	int	 m_strafe_sequence;
	bool m_strafe_changing;
	float m_duration_strafing;
	float m_foot_lerp;
	bool m_feet_crossed;
	bool m_player_is_accelerating;
	PAD ( 372 );
	float m_camera_smooth_height;
	bool m_smooth_height_valid;
	PAD ( 16 );
	float m_aim_yaw_min;
	float m_aim_yaw_max;
	int	m_state_version;
};

enum class life_states_t : int {
	life_alive = 0,
	life_dying,
	life_dead,
	life_reborn,
	life_discard
};

enum flags_t : int {
	on_ground = ( 1 << 0 ),
	ducking = ( 1 << 1 ),
	water_jump = ( 1 << 2 ),
	on_train = ( 1 << 3 ),
	in_rain = ( 1 << 4 ),
	frozen = ( 1 << 5 ),
	at_controls = ( 1 << 6 ),
	client = ( 1 << 7 ),
	fake_client = ( 1 << 8 ),
};

struct datamap_t;
struct inputdata_t;

typedef void ( player_t:: *inputfunc_t )( inputdata_t &data );

enum {
	td_offset_normal = 0,
	td_offset_packed = 1,
	td_offset_count,
};

enum {
	pc_non_networked_only = 0,
	pc_networked_only,
	pc_copytype_count,
	pc_everything = pc_copytype_count,
};

struct typedescription_t {
	int	m_type;
	const char *m_field_name;
	int	m_field_offset;
	unsigned short m_field_size;
	short m_flags;
	const char *m_ext_name;
	void *m_save_restore_ops;
	inputfunc_t	m_input_func;
	datamap_t *m_td;
	int	m_field_size_in_bytes;
	struct typedescription_t *m_override_field;
	int	m_override_count;
	float m_field_tolerance;
	int	m_flat_offset [ td_offset_count ];
	unsigned short m_flat_group;
};

struct optimized_datamap_t;

struct datamap_t {
	typedescription_t *m_data_desc;
	int	m_data_num_fields;
	const char *m_data_class_name;
	datamap_t *m_base;
};

class player_t : public entity_t {
public:
	OFFSET ( int, button_pressed, 0x31E0 );
	OFFSET ( int, button_last, 0x31DC );
	OFFSET ( int, button_released, 0x31E4 );
	OFFSET ( int, button_disabled, 0x3340 );
	OFFSET ( int, sequence, 0xA30 );
	NETVAR ( int, survival_team, "DT_CSPlayer", "m_nSurvivalTeam" );
	OFFSET ( animstate_t *, animstate, 0x3874 );
	NETVAR ( int, health, "DT_BasePlayer", "m_iHealth" );
	NETVAR ( life_states_t, life_state, "DT_BasePlayer", "m_lifeState" );
	NETVAR ( int, tick_base, "DT_BasePlayer", "m_TickBase" );
	NETVAR ( uint32_t, handle, "DT_BasePlayer", "m_hGroundEntity" );
	NETVAR ( flags_t , flags, "DT_BasePlayer", "m_fFlags" );
	NETVAR_ADDITIVE ( vec_t, mins, "DT_BaseEntity", "m_Collision", 8 );
	NETVAR_ADDITIVE ( vec_t, maxs, "DT_BaseEntity", "m_Collision", 20 );
	NETVAR ( uint32_t, weapon_handle, "DT_BaseCombatCharacter", "m_hActiveWeapon" );
	NETVAR ( int, armor, "DT_CSPlayer", "m_ArmorValue" );
	NETVAR ( bool, helmet, "DT_CSPlayer", "m_bHasHelmet" );
	NETVAR ( bool, heavy_armor, "DT_CSPlayer", "m_bHasHeavyArmor" );
	OFFSET ( bool, use_new_animstate, 0x39E1 );
	OFFSET ( int, button_forced, 0x3310 );
	NETVAR ( float, crouch_amount, "DT_BasePlayer", "m_flDuckAmount" );
	NETVAR ( float, crouch_speed, "DT_BasePlayer", "m_flDuckSpeed" );
	NETVAR ( vec_t, vel, "DT_BasePlayer", "m_vecVelocity[0]" );
	NETVAR ( vec_t, base_vel, "DT_BasePlayer", "m_vecBaseVelocity" );
	NETVAR ( float, velocity_modifier, "DT_CSPlayer", "m_flVelocityModifier" );
	NETVAR ( vec_t, view_offset, "DT_BasePlayer", "m_vecViewOffset[0]" );
	NETVAR ( vec_t, aim_punch, "DT_BasePlayer", "m_aimPunchAngle" );
	NETVAR ( vec_t, view_punch, "DT_BasePlayer", "m_viewPunchAngle" );
	NETVAR ( vec_t, aim_punch_vel, "DT_BasePlayer", "m_aimPunchAngleVel" );

	__forceinline bool alive ( ) {
		return this && life_state ( ) == life_states_t::life_alive;
	}

	void set_abs_origin ( const vec_t &origin );
	void set_abs_angles ( const vec_t &angles );
	int lookup_bone ( const char *name );
	void select_item ( const char *name, int sub_type );
	void modify_eye_position ( animstate_t *state, vec_t *input_eye_pos );
	bool is_enemy ( player_t *other );
	weapon_t *weapon ( );
	vec_t shoot_pos ( );
	datamap_t *get_pred_desc_map ( );
	void pre_think ( );
	bool is_player ( );
	void think ( );
	void set_sequence ( int sequence );
	std::uint8_t *get_original_network_data_object ( ) const;
	std::uint8_t *get_predicted_frame ( int frame_num );
	bool physics_run_think ( int think_method );
	void bone_pos ( int bone, vec_t &out, vec_t &q );
};
