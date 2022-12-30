#pragma once

struct prediction_data_t {
	int m_tick_base;
	flags_t m_flags;
	movetypes_t m_movetype;
	uint32_t m_ground_entity;
	float m_crouch_amount;
	float m_crouch_speed;
	float m_recoil_index;
	vec_t m_origin;
	vec_t m_velocity;
	vec_t m_base_velocity;
	float m_fall_velocity;
	float m_velocity_modifier;
	vec_t m_view_offset;
	vec_t m_aim_punch_angle;
	vec_t m_view_punch_angle;
	vec_t m_aim_punch_angle_vel;
	float m_accuracy_penalty;
};

struct inputpred_t {
	void pre_create_move ( );
	void predict ( ucmd_t *ucmd, const std::function<void ( )> &fn );
	void begin ( ucmd_t *ucmd );
	void end ( ucmd_t *ucmd );
	void restore_to_pre_prediction ( );
	void restore_to_post_prediction ( );
	void restore ( prediction_data_t *data, player_t *player );
	void store ( prediction_data_t *data, player_t *player );
	void fix_velocity_modifier ( );
	void force_update ( bool error );

	typedescription_t *find_flat_field_by_name ( const char *fieldname, const datamap_t *dmap );

	uintptr_t m_prediction_player;
	uintptr_t m_prediction_seed;
	bool m_in_prediction;
	bool m_is_first_time_predicted;
	float m_curtime;
	float m_frametime;
	bool m_has_error = false;
	vec_t m_vel;
	flags_t m_flags;
	c_move_data data;

	byte *m_start_data;
	byte *m_end_data;

	float m_vel_modifier = 1.0f;
	float m_old_vel_modifier = 1.0f;
};

extern inputpred_t prediction;