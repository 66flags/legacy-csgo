#pragma once

class c_panel {
	enum indices_t : std::uintptr_t {
		_get_name = 36,
	};
public:
	__forceinline const char *get_name ( unsigned int panel ) {
		using get_name_fn = const char *( __thiscall * ) ( void *, unsigned int );
		return util::get_method < get_name_fn > ( this, indices_t::_get_name )( this, panel );
	}
};