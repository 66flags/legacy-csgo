#include "includes.h"

HRESULT __stdcall hooks_t::reset ( IDirect3DDevice9 *dev, D3DPRESENT_PARAMETERS *pp ) {
	const auto hr = hooks.m_reset.get_old_method< decltype( &reset ) > ( )( dev, pp );

	ImGui_ImplDX9_InvalidateDeviceObjects ( );

	if ( SUCCEEDED ( hr ) ) {
		ImGui_ImplDX9_CreateDeviceObjects ( );
	}

	return hr;
}