#include "includes.h"

weapon_info_t *weapon_t::data ( ) {
	using data_fn = weapon_info_t * ( __thiscall * ) ( void * );
	return util::get_method < data_fn > ( this, 446 ) ( this );
}