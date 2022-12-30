#pragma once

class c_entlist {
	enum indices_t : std::uintptr_t {
		_get = 3,
		_get_by_handle = 4,
	};
public:
	template < typename t >
	__forceinline t get ( int idx ) {
		return util::get_method< t ( __thiscall * )( void *, int ) > ( this, indices_t::_get )( this, idx );
	}

	template < typename t >
	__forceinline t get_by_handle ( std::uint32_t handle ) {
		return util::get_method< t ( __thiscall * )( void *, std::uint32_t ) > ( this, indices_t::_get_by_handle )( this, handle );
	}
};