#pragma once

class c_client_class;
class c_recv_table;
class c_recv_prop;
class c_recv_proxy_data;
class c_variant;

using create_client_class_fn = void *( * )( int, int );
using create_event_fn = void *( * )( );

using recv_var_proxy_fn = void ( * )( const c_recv_proxy_data *, void *, void * );
using array_length_recv_proxy_fn = void ( * )( void *, int, int );
using data_table_recv_var_proxy_fn = void ( * )( const c_recv_prop *, void **, void *, int );

enum send_prop_type_t {
	dpt_int = 0,
	dpt_float,
	dpt_vector,
	dpt_vectorxy,
	dpt_string,
	dpt_array,
	dpt_datatable,
	dpt_int64,
	dpt_numsendproptypes
};

class c_variant {
public:
	union {
		float f;
		long i;
		char *s;
		void *data;
		float v [ 3 ];
		long long i64;
	};

	send_prop_type_t m_type;
};

class c_recv_proxy_data {
public:
	const c_recv_prop *m_recv_prop;
	c_variant m_value;
	int	m_element;
	int	m_object_id;
};

class c_recv_prop {
public:
	char *m_var_name;
	send_prop_type_t m_recv_type;
	int m_flags;
	int m_string_buffer_size;
	bool m_inside_array;
	const void *m_extra_data;
	c_recv_prop *m_array_prop;
	array_length_recv_proxy_fn m_array_length_proxy;
	recv_var_proxy_fn m_proxy_fn;
	data_table_recv_var_proxy_fn m_data_table_proxy_fn;
	c_recv_table *m_data_table;
	int m_offset;
	int m_element_stride;
	int m_num_elements;
	const char *m_parent_array_prop_name;
};

class c_client_class {
public:
	create_client_class_fn m_create_fn;
	create_event_fn m_create_event_fn;
	char *m_network_name;
	c_recv_table *m_recv_table;
	c_client_class *m_next;
	int	m_class_id;
	const char *m_map_class_name;
};

class c_recv_table {
public:
	c_recv_prop *m_props;
	int	m_num_props;
	void *m_decoder;
	char *m_net_table_name;
	bool m_initialized;
	bool m_in_main_list;
};

class client_class_t;

class c_net_channel_info {
	enum indices_t : std::uintptr_t {
		_get_latency = 9,
		_get_avg_latency = 10,
	};
public:
	float get_latency ( int flow ) {
		using get_latency_fn = float ( __thiscall * )( void *, int );
		return util::get_method< get_latency_fn > ( this, indices_t::_get_latency )( this, flow );
	}

	float get_avg_latency ( int flow ) {
		using get_avg_latency_fn = float ( __thiscall * )( void *, int );
		return util::get_method< get_avg_latency_fn > ( this, indices_t::_get_avg_latency )( this, flow );
	}
};

class c_net_channel {
	enum indices_t : std::uintptr_t {
		_send_net_msg = 40,
		_send_datagram = 46,
	};
public:
	PAD ( 18 );
	int m_out_sequence_nr;
	int m_im_sequence_nr;
	int m_out_sequence_nr_ack;
	int m_out_reliable_state;
	int m_in_reliable_state;
	int m_choked_packets;

	bool send_net_msg ( void *msg, bool reliable, bool voice ) {
		using send_net_msg_fn = bool ( __thiscall * )( void *, void *, bool, bool );
		return util::get_method< send_net_msg_fn > ( this, indices_t::_send_net_msg )( this, msg, reliable, voice );
	}

	void send_datagram ( ) {
		using send_datagram_fn = int ( __thiscall * )( void *, void * );
		util::get_method< send_datagram_fn > ( this, indices_t::_send_datagram )( this, 0 );
	}
};

class client_event_t {
public:
	enum {
		event_index_bits = 8,
		event_data_len_bits = 11,
		max_event_data = 192,
	};

	short m_class_id;
	float m_fire_delay;
	const void *m_send_table;
	const client_class_t *m_client_class;
	int	m_bits;
	uint8_t *m_data;
	int	m_flags;
	PAD ( 0x1C );
	client_event_t *m_next;
};

class c_client_state {
public:
	PAD ( 156 );
	c_net_channel *m_net_channel;
	int m_challenge_nr;
	PAD ( 4 );
	double m_connect_time;
	int m_retry_number;
	PAD ( 84 );
	int m_signon_state;
	PAD ( 4 );
	double m_next_cmd_time;
	int m_server_count;
	int m_current_sequence;
	PAD ( 8 );

	struct {
		float m_clock_offsets [ 16 ];
		int m_cur_clock_offset;
		int m_server_tick;
		int m_client_tick;
	} clock_drift_mgr;

	int m_delta_tick;
	PAD ( 4 );
	int m_view_entity;
	int m_player_slot;
	bool m_paused;
	PAD ( 3 );
	char m_level_name [ 260 ];
	char m_level_name_short [ 40 ];
	PAD ( 212 );
	int m_max_clients;
	PAD ( 18836 );
	int m_old_tickcount;
	float m_tick_remainder;
	float m_frametime;
	int m_last_outgoing_command;
	int m_choked_commands;
	int m_last_command_ack;
	int m_last_server_tick;
	int m_command_ack;
	int m_sound_sequence;
	int m_last_progress_percent;
	bool m_is_hltv;
	PAD ( 75 );
	vec_t m_ang;
	PAD ( 204 );
	client_event_t *m_events;
};

class c_client {
	enum indices_t : std::uintptr_t {
		_create_move = 22,
		_frame_stage_notify = 37,
		_get_all_classes = 8
	};
public:
	__forceinline c_client_class *get_all_classes ( ) {
		using get_all_classes_fn = c_client_class * ( __thiscall * )( void * );
		return util::get_method< get_all_classes_fn > ( this, indices_t::_get_all_classes )( this );
	}
};