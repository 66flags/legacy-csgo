#include "includes.h"

void __fastcall hooks_t::get_color_modulation ( void *ecx, void *edx, int *r, int *g, int *b ) {
	//const auto mat = reinterpret_cast < material_t * > ( ecx );

	//if ( !mat || mat->is_error_material ( ) || strstr ( mat->get_texture_group_name ( ), _ ( "StaticProp textures" ) ) )
	//	return hooks.m_get_color_modulation.get_old_method < decltype ( &get_color_modulation ) > ( ) ( ecx, edx, r, g, b );

	//if ( settings.get_item < bool > ( _ ( "visuals.modulate_world" ) ) && strstr ( mat->get_texture_group_name ( ), _ ( "World textures" ) ) ) {
	//	static auto &modulate_world_color = settings.get_item < sesui::color > ( _ ( "visuals.modulate_world_color" ) );

	//	hooks.m_get_color_modulation.get_old_method < decltype ( &get_color_modulation ) > ( ) ( ecx, edx, r, g, b );

	//	*r = static_cast < float > ( 0.23f * ( modulate_world_color.r * 255.f ) );
	//	*g = static_cast < float > ( 0.23f * ( modulate_world_color.g * 255.f ) );
	//	*b = static_cast < float > ( 0.23f * ( modulate_world_color.b * 255.f ) );
	//}
	//else {
	//	return hooks.m_get_color_modulation.get_old_method < decltype ( &get_color_modulation ) > ( ) ( ecx, edx, r, g, b );
	//}
}