#include "includes.h"

HRESULT __stdcall hooks_t::end_scene ( IDirect3DDevice9 *device ) {
	static auto ret = _ReturnAddress ( );

	if ( ret != _ReturnAddress ( ) )
		return hooks.m_end_scene.get_old_method < decltype( &hooks_t::end_scene ) > ( )( device );

	/* update input every frame update. */
	input::update_input ( );

	menu.begin ( device, [ ] {
		menu.draw ( );
	} );

	return hooks.m_end_scene.get_old_method < decltype( &hooks_t::end_scene ) > ( )( device );
}
