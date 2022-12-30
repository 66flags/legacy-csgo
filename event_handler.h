#pragma once

class c_event_handler : public c_event_listener {
public:
	c_event_handler ( );
	~c_event_handler ( );

	virtual void fire_game_event ( event_t *event );
	int get_event_debug_id ( );
};