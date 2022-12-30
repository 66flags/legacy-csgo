#include "includes.h"

void __fastcall hooks_t::paint_traverse ( void *ecx, void *edx, unsigned int panel, bool force_repaint, bool allow_force ) {
	const auto panel_name_hash = HASH ( interfaces::m_panel->get_name ( panel ) );

	hooks.m_paint_traverse.get_old_method < decltype ( &paint_traverse ) > ( )( ecx, edx, panel, force_repaint, allow_force );

	if ( panel_name_hash == HASH ( "FocusOverlayPanel" ) ) {
	}
}