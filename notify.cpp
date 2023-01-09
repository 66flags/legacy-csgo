#include "includes.h"

notify_t notify { };

void notify_t::print ( const std::string &msg ) {
	interfaces::m_cvar->console_color_printf ( m_accent, _ ( "[hypnotic] " ) );
	interfaces::m_cvar->console_color_printf ( m_accent, msg.data ( ) );
}

void notify_t::add ( const std::string &msg ) {
	print ( msg );

	m_notify_text.push_front ( { msg, m_accent, interfaces::m_globals->m_curtime, 255 } );
}

void notify_t::paint ( ) {
	if ( m_notify_text.empty ( ) )
		return;

	for ( int i = 0; i < m_notify_text.size ( ); i++ ) {
		auto &notify = m_notify_text [ i ];
	}
}