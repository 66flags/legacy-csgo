#include "includes.h"

HRESULT __stdcall hooks_t::reset ( IDirect3DDevice9 *dev, D3DPRESENT_PARAMETERS *pp ) {
	const auto hr = hooks.m_reset.get_old_method< decltype( &reset ) > ( )( dev, pp );

	if ( SUCCEEDED ( hr ) ) {
		if ( sesui::style.control_font.data ) {
			reinterpret_cast< ID3DXFont * > ( sesui::style.control_font.data )->Release ( );
			sesui::style.control_font.data = nullptr;
		}
	}

	return hr;
}