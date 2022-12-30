#pragma once

class c_econ_item_definition {
public:
	virtual const char *idx ( ) = 0;
	virtual const char *prefab_name ( ) = 0;
	virtual const char *item_base_name ( ) = 0;
};

class c_localize {
	enum indices_t : uintptr_t {
		_find = 11,
	};
public:
	wchar_t *find ( const char *token_name ) {
		using find_fn = wchar_t *( __thiscall * )( void *, const char * );
		return util::get_method < find_fn > ( this, indices_t::_find )( this, token_name );
	}
};