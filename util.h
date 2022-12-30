#pragma once

namespace util {
	template < typename t >
	__forceinline t get_method ( void *thisptr, std::uintptr_t idx ) {
		return reinterpret_cast< t >( ( *reinterpret_cast< std::uintptr_t ** >( thisptr ) ) [ idx ] );
	}
}