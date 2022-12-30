#include "includes.h"

void __fastcall hooks_t::paint ( void *ecx, void *edx, paint_modes_t mode ) {
	const auto engine_vgui = reinterpret_cast< c_engine_vgui * >( ecx );

	hooks.m_paint.get_old_method < decltype ( &paint ) > ( )( ecx, edx, mode );

	if ( engine_vgui->m_static_transition_panel && ( mode & paint_modes_t::paint_uipanels ) ) {
		interfaces::m_surface->paint ( [ & ] {
			visuals.paint ( );
			notify.paint ( );
		} );
	}
}