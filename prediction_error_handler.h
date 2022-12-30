#pragma once

struct prediction_error_handler_t {
	void post_data_network_received ( player_t* entity, int commands_acknowledged );
	void handle ( const char* field_name );
};

extern prediction_error_handler_t prediction_error_handler;