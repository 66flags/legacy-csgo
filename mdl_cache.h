#pragma once

class c_model_cache {
	enum indices_t : uintptr_t {
		_begin_lock = 32,
		_end_lock = 33,
	};
public:
	__forceinline void begin_lock ( ) {
		using begin_lock_fn = void ( __thiscall * )( void * );
		util::get_method< begin_lock_fn > ( this, indices_t::_begin_lock )( this );
	}

	__forceinline void end_lock ( ) {
		using end_lock_fn = void ( __thiscall * )( void * );
		util::get_method< end_lock_fn > ( this, indices_t::_end_lock )( this );
	}
};