#pragma once

class player_t;

class c_move_helper {
public:
	virtual void pad00 ( ) = 0;
	virtual void set_host ( void *host ) = 0;
	virtual void pad01 ( ) = 0;
	virtual void pad02 ( ) = 0;
	virtual void process_impacts ( ) = 0;
};

class c_move_data {
public:
	bool m_first_run_of_functions : 1;
	bool m_game_code_moved_player : 1;
	bool m_no_air_control : 1;
	unsigned long m_player_handle;
	int	m_impulse_command;
	vec_t m_view_angles;
	vec_t m_abs_view_angles;
	int	m_buttons;
	int m_old_buttons;
	float m_fmove;
	float m_smove;
	float m_umove;
	float m_max_speed;
	float m_client_max_speed;
	vec_t m_vel;
	vec_t m_trailing_vel;
	float m_trailing_vel_time;
	vec_t m_ang;
	vec_t m_old_ang;
	float m_step_height;
	vec_t m_wish_vel;
	vec_t m_jump_vel;
	vec_t m_constraint_center;
	float m_constraint_radius;
	float m_constraint_width;
	float m_constraint_speed_factor;
	bool m_constraint_past_radius;
	vec_t m_abs_origin;
};

class c_prediction {
	enum indices_t : std::uintptr_t {
		_update = 3,
		_set_local_view_angles = 13,
		_check_moving_ground = 18,
		_setup_move = 20,
		_finish_move = 21,
	};
public:
	void update ( int start_frame, bool valid_frame, int incoming_acknowledged, int outgoing_cmd ) {
		using update_fn = void ( __thiscall * )( void *, int, bool, int, int );
		util::get_method < update_fn > ( this, indices_t::_update )( this, start_frame, valid_frame, incoming_acknowledged, outgoing_cmd );
	}
	
	void check_moving_ground ( player_t *player, double frametime ) {
		using check_moving_ground_fn = void ( __thiscall * )( void *, player_t *, double );
		util::get_method < check_moving_ground_fn > ( this, indices_t::_check_moving_ground )( this, player, frametime );
	}

	void setup_move ( player_t *player, ucmd_t *ucmd, void *move_helper, c_move_data *move ) {
		using setup_move_fn = void ( __thiscall * )( void *, player_t *, ucmd_t *, void *, c_move_data * );
		util::get_method < setup_move_fn > ( this, indices_t::_setup_move )( this, player, ucmd, move_helper, move );
	}

	void set_local_view_angles ( vec_t &ang ) {
		using set_local_view_angles_fn = void ( __thiscall * )( void *, vec_t & );
		util::get_method < set_local_view_angles_fn > ( this, indices_t::_set_local_view_angles )( this, ang );
	}

	void finish_move ( player_t *player, ucmd_t *ucmd, c_move_data *move ) {
		using finish_move_fn = void ( __thiscall * )( void *, player_t *, ucmd_t *, c_move_data * );
		util::get_method < finish_move_fn > ( this, indices_t::_finish_move )( this, player, ucmd, move );
	}

	PAD ( 4 );
	std::uintptr_t m_last_ground;
	bool m_in_prediction;
	bool m_is_first_time_predicted;
	bool m_engine_paused;
	bool m_old_pred;
	int m_prev_start_frame;	
	int	m_incoming_packet_num;
	PAD ( 8 ); 
	int	m_cmds_predicted;				 
	int	m_server_commands_ack;	 
	int	m_prev_ack_had_errors;			 
	float m_ideal_pitch;					 
	int	m_last_command_ack;			 
};

class c_game_movement {
public:
	virtual ~c_game_movement ( void ) = 0;
	virtual void process_movement ( void *player, void *move ) = 0;
	virtual void reset ( void ) = 0;
	virtual void start_track_prediction_errors ( void *player ) = 0;
	virtual void finish_track_prediction_errors ( void *player ) = 0;
	virtual void diff_print ( char const *fmt, ... ) = 0;
	virtual vec_t const &get_player_mins ( bool ducked ) const = 0;
	virtual vec_t const &get_player_maxs ( bool ducked ) const = 0;
	virtual vec_t const &get_player_view_offset ( bool ducked ) const = 0;
	virtual bool is_moving_player_stuck ( void ) const = 0;
	virtual void *get_moving_player ( void ) const = 0;
	virtual void unblock_pusher ( void *player, void *pusher ) = 0;
	virtual void setup_movement_bounds ( void *move ) = 0;
};