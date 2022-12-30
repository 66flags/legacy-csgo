#pragma once

struct netvars_t {
	int get_offset ( hash32_t table, hash32_t prop );
	
	struct netvar_data_t {
		bool m_datamap_var;
		c_recv_prop *m_prop_ptr;
		size_t m_offset;
	};

	std::unordered_map < hash32_t, std::unordered_map < hash32_t, netvar_data_t > > m_offsets;

	bool init ( );
	void store_table ( const std::string &name, c_recv_table *table, std::size_t offset = 0 );
};

#define NETVAR( t, func, table, prop ) \
__forceinline t& func ( ) { \
	static auto offset = netvars.get_offset ( HASH ( table ), HASH ( prop ) ); \
	return *( t* )( std::uintptr_t ( this ) + offset ); \
}

#define NETVAR_ADDITIVE( t, func, table, prop, off ) \
__forceinline t& func ( ) { \
	static auto offset = netvars.get_offset ( HASH ( table ), HASH ( prop ) ); \
	return *( t* )( ( std::uintptr_t ( this ) + offset ) + off ); \
}

#define OFFSET( t, func, offset ) \
__forceinline t& func ( ) { \
	return *( t* )( std::uintptr_t ( this ) + offset ); \
}

#define POFFSET( t, func, offset ) \
__forceinline t* func( ) { \
    return ( t* )(  std::uintptr_t ( this ) + offset ); \
}

extern netvars_t netvars;