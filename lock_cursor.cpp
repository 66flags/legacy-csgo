#include "includes.h"

void __fastcall hooks_t::lock_cursor ( void *ecx, void *edx ) {
	if ( gui.m_visible ) {
		interfaces::m_surface->unlock_cursor ( );
		return;
	}

	hooks.m_lock_cursor.get_old_method< decltype( &lock_cursor ) > ( )( ecx, edx );
}