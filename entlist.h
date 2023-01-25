#pragma once

class c_entlist {
	enum indices_t : std::uintptr_t {
		_get = 3,
		_get_by_handle = 4,
		_get_highest_entity_index = 6,
	};
public:
	__forceinline int get_highest_entity_index ( ) {
		return util::get_method < int ( __thiscall * )( void * ) > ( this, indices_t::_get_highest_entity_index )( this );
	}

	template < typename t >
	__forceinline t get ( int idx ) {
		return util::get_method < t ( __thiscall * )( void *, int ) > ( this, indices_t::_get )( this, idx );
	}

	template < typename t >
	__forceinline t get_by_handle ( std::uint32_t handle ) {
		return util::get_method < t ( __thiscall * )( void *, std::uint32_t ) > ( this, indices_t::_get_by_handle )( this, handle );
	}
};