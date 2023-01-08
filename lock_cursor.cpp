#include "includes.h"

void __fastcall hooks_t::lock_cursor ( void *ecx, void *edx ) {
	if ( menu.m_opened ) {
		interfaces::m_surface->unlock_cursor ( );
		return;
	}

	hooks.m_lock_cursor.get_old_method< decltype( &lock_cursor ) > ( )( ecx, edx );
}