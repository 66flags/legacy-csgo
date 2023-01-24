#include "includes.h"

float __fastcall hooks_t::get_alpha_modulation ( void *ecx, void *edx ) {
	const auto mat = reinterpret_cast < material_t * > ( ecx );

	if ( !mat || mat->is_error_material ( ) || !strstr ( mat->get_texture_group_name ( ), _ ( "StaticProp textures" ) ) )
		return hooks.m_get_alpha_modulation.get_old_method < decltype ( &get_alpha_modulation ) > ( ) ( ecx, edx );

	return 1.0f - ( settings.get_item < int > ( _ ( "visuals.transparent_props_amount" ) ) / 100.f );
}