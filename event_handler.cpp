#include "includes.h"

c_event_handler::c_event_handler ( ) {
	interfaces::m_event_mgr->add_listener ( this, _ ( "player_hurt" ), false );
	interfaces::m_event_mgr->add_listener ( this, _ ( "player_death" ), false );
	interfaces::m_event_mgr->add_listener ( this, _ ( "bullet_impact" ), false );
}

c_event_handler::~c_event_handler ( ) {
	interfaces::m_event_mgr->remove_listener ( this );
}

void c_event_handler::fire_game_event ( event_t *event ) {
	if ( !event || !g.m_local )
		return;
}

int c_event_handler::get_event_debug_id ( ) {
	return 42;
}