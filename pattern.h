#pragma once

class pattern {
public:
	std::uint8_t *m_addr;
public:
	__forceinline pattern ( std::uint8_t *addr ) {
		m_addr = addr;
	}

	template < typename t >
	__forceinline t as ( ) {
		return t ( m_addr );
	}

	__forceinline pattern sub ( uintptr_t bytes ) {
		return pattern ( m_addr - bytes );
	}

	__forceinline pattern add ( std::uintptr_t bytes ) {
		return pattern ( m_addr + bytes );
	}

	__forceinline pattern deref ( ) {
		return pattern ( *reinterpret_cast< pattern * >( m_addr ) );
	}

	__forceinline pattern rel32 ( ) {
		return pattern ( m_addr + *reinterpret_cast< int * > ( m_addr + 1 ) + 5 );
	}

	static __declspec( noinline ) pattern find ( const char *mod, const char *signature ) {
		static auto pattern_to_byte = [ ] ( const char *pattern ) {
			auto bytes = std::vector< int > { };
			auto start = const_cast< char * >( pattern );
			auto end = const_cast< char * >( pattern ) + strlen ( pattern );

			for ( auto current = start; current < end; ++current ) {
				if ( *current == '?' ) {
					++current;

					if ( *current == '?' )
						++current;

					bytes.push_back ( -1 );
				}
				else
					bytes.push_back ( strtoul ( current, &current, 16 ) );
			}

			return bytes;
		};

		auto handle = LI_FN ( GetModuleHandleA )( mod );
		auto dos_headers = ( PIMAGE_DOS_HEADER ) handle;
		auto headers = ( PIMAGE_NT_HEADERS ) ( ( std::uint8_t * ) handle + dos_headers->e_lfanew );

		auto image_size = headers->OptionalHeader.SizeOfImage;
		auto pattern_bytes = pattern_to_byte ( signature );
		auto scan_bytes = reinterpret_cast< std::uint8_t * >( handle );

		auto s = pattern_bytes.size ( );
		auto d = pattern_bytes.data ( );

		for ( auto i = 0ul; i < image_size - s; ++i ) {
			bool found = true;

			for ( auto j = 0ul; j < s; ++j ) {
				if ( scan_bytes [ i + j ] != d [ j ] && d [ j ] != -1 ) {
					found = false;
					break;
				}
			}

			if ( found )
				return &scan_bytes [ i ];
		}

		return pattern ( 0 );
	}
};