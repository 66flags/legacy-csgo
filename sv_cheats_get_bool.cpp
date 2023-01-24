#include "includes.h"

bool __fastcall hooks_t::sv_cheats_get_bool ( void *ecx, void *edx ) {
	static auto cam_think = pattern::find ( _ ( "client.dll" ), _ ( "85 C0 75 30 38 86" ) ).as < void * > ( );

	if ( _ReturnAddress ( ) == cam_think )
		return true;

	return hooks.m_sv_cheats_get_bool.get_old_method < decltype ( &sv_cheats_get_bool ) > ( ) ( ecx, edx );
}