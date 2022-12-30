#include "includes.h"

netvars_t netvars { };

bool netvars_t::init ( ) {
	auto client_class = interfaces::m_client->get_all_classes ( );

	if ( !client_class )
		return false;

	for ( ; client_class != nullptr; client_class = client_class->m_next )
		store_table ( client_class->m_recv_table->m_net_table_name, client_class->m_recv_table );

	return true;
}

void netvars_t::store_table ( const std::string &name, c_recv_table *table, std::size_t offset ) {
	hash32_t base = HASH ( name );

	for ( auto i = 0; i < table->m_num_props; i++ ) {
		auto prop = &table->m_props [ i ];
		auto child = prop->m_data_table;

		if ( child && child->m_num_props > 0 )
			store_table ( name, prop->m_data_table, prop->m_offset + offset );

		const auto var = HASH ( prop->m_var_name );

		if ( !m_offsets [ base ][ var ].m_offset ) {
			m_offsets [ base ][ var ].m_datamap_var = false;
			m_offsets [ base ][ var ].m_prop_ptr = prop;
			m_offsets [ base ][ var ].m_offset = static_cast < std::size_t > ( prop->m_offset + offset );
		}
	}
}

int netvars_t::get_offset ( hash32_t table, hash32_t prop ) {
	return m_offsets [ table ][ prop ].m_offset;
}