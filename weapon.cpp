#include "includes.h"

weapon_info_t *weapon_t::data ( ) {
	using data_fn = weapon_info_t * ( __thiscall * ) ( void * );
	return util::get_method < data_fn > ( this, 446 ) ( this );
}

const wchar_t *weapon_t::name ( ) {
	auto item_definition = static_data ( econ_item_view ( this ) );

	if ( !item_definition )
		return L"ERROR";

	return interfaces::m_localize->find ( item_definition->item_base_name ( ) );
}

c_econ_item_definition *weapon_t::econ_item_view ( weapon_t *weapon ) {
	using econ_item_view_fn = c_econ_item_definition * ( __thiscall * )( void * );
	static auto econ_item_view = pattern::find ( _ ( "client.dll" ), _ ( "8B 81 ? ? ? ? 81 C1 ? ? ? ? FF 50 04 83 C0 40 C3" ) ).as < econ_item_view_fn > ( );
	return econ_item_view ( weapon );
}

c_econ_item_definition *weapon_t::static_data ( c_econ_item_definition *item ) {
	using static_data_fn = c_econ_item_definition * ( __thiscall * )( void * );
	static auto static_data = pattern::find ( _ ( "client.dll" ), _ ( "55 8B EC 51 56 57 8B F1 E8 ? ? ? ? 0F B7 8E ? ? ?" ) ).as < static_data_fn > ( );
	return static_data ( item );
}