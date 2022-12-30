#include "includes.h"

c_prediction_copy::c_prediction_copy ( int type, byte *dest, bool dest_packed, const byte *src, bool src_packed, optype_t optype, FN_FIELD_COMPARE func /*= NULL*/ ) {
	m_optype = optype;
	m_num_type = type;
	m_dest = dest;
	m_src = src;
	m_destoffsetindex = dest_packed ? td_offset_packed : td_offset_normal;
	m_srcoffsetindex = src_packed ? td_offset_packed : td_offset_normal;
	m_error_count = 0;
	m_entindex = -1;
	m_watch_field = 0;
	m_field_compare_fn = func;
}

int c_prediction_copy::transfer_data ( const char *operation, int idx, datamap_t *dmap ) {
	using transfer_data_fn = int ( __thiscall * )( c_prediction_copy *, const char *, int, datamap_t * );
	static auto o_transfer_data = pattern::find ( _ ( "client.dll" ), _ ( "55 8B EC 8B 45 10 53 56 8B F1 57 8B 7D 0C 89 7E 1C 85" ) ).as < transfer_data_fn > ( );
	return o_transfer_data ( this, operation, idx, dmap );
}

inline typedescription_t *c_prediction_copy::find_flat_field_by_name ( const char *field_name, const datamap_t *datamap ) {
	using find_flat_field_by_name_fn = typedescription_t * ( __fastcall * )( const char *, const datamap_t * );
	static auto o_find_flat_field_by_name = pattern::find ( _ ( "client.dll" ), _ ( "E8 ? ? ? ? 8B CF 8B 40 34 03 45 08 F3 0F 7E 00" ) ).rel32 ( ).as< find_flat_field_by_name_fn > ( );
	return o_find_flat_field_by_name ( field_name, datamap );
}