#pragma once

class cvar_t;

enum cvar_flags_t : uint32_t {
	fvar_unregistered = ( 1 << 0 ),
	fvar_developmentonly = ( 1 << 1 ),
	fvar_gamedll = ( 1 << 2 ),
	fvar_clientdll = ( 1 << 3 ),
	fvar_hidden = ( 1 << 4 ),
	fvar_protected_var = ( 1 << 5 ),
	fvar_sponly = ( 1 << 6 ),
	fvar_archive = ( 1 << 7 ),
	fvar_notify = ( 1 << 8 ),
	fvar_userinfo = ( 1 << 9 ),
	fvar_printableonly = ( 1 << 10 ),
	fvar_unlogged = ( 1 << 11 ),
	fvar_never_as_string = ( 1 << 12 ),
};

struct cvar_value_t {
	char *m_str;
	int m_str_len;
	float m_fvalue;
	int	m_ivalue;
};

using change_callback_t = void( * )( cvar_t *var, const char *old_value, float float_old_value );

class cvar_t {
	enum indices_t : std::uintptr_t {
		_get_name = 5,
		_set_int = 16,
		_set_float = 15,
		_set_char = 14,
	};
private:
	void *m_vtable;
public:
	cvar_t *m_next;
	int m_registered;
	char *m_name;
	char *m_help_string;
	int m_flags;
	void *m_callback;
	cvar_t *m_parent;
	char *m_default_value;
	cvar_value_t m_value;
	int m_has_min;
	float m_min;
	int m_has_max;
	float m_max;
	void *m_callbacks;
public:
	__forceinline void set_int ( int value ) {
		using set_int_fn = void ( __thiscall * )( void *, int );
		util::get_method< set_int_fn > ( this, indices_t::_set_int )( this, value );
	}

	__forceinline void set_float ( float value ) {
		using set_float_fn = void ( __thiscall * )( void *, float );
		util::get_method< set_float_fn > ( this, indices_t::_set_float )( this, value );
	}

	__forceinline void set_char ( const char *value ) {
		using set_char_fn = void ( __thiscall * )( void *, const char * );
		util::get_method< set_char_fn > ( this, indices_t::_set_char )( this, value );
	}

	__forceinline void remove_callbacks ( ) {
		*reinterpret_cast< int * > ( reinterpret_cast< uintptr_t > ( &m_callbacks ) + 0xC ) = 0;
	}

	__forceinline const char *get_name ( ) {
		using get_name_fn = const char * ( __thiscall * ) ( void * );
		return util::get_method < get_name_fn > ( this, indices_t::_get_name )( this );
	}

	__forceinline const char *get_string ( ) {
		if ( m_flags & cvar_flags_t::fvar_never_as_string )
			return "FCVAR_NEVER_AS_STRING";

		return m_value.m_str ? m_value.m_str : "";
	}

	__forceinline bool get_bool ( ) {
		return !!get_int ( );
	}

	__forceinline float get_float ( ) {
		uint32_t xored = *reinterpret_cast< uintptr_t * >( &m_value.m_fvalue ) ^ reinterpret_cast< uintptr_t >( this );
		return *reinterpret_cast< float * >( &xored );
	}

	__forceinline int get_int ( ) {
		return static_cast< int > ( m_value.m_ivalue ^ reinterpret_cast< uintptr_t >( this ) );
	}
};

class c_convar {
public:
	__forceinline void console_color_printf ( const color_t &col, const char *format, ... ) {
		using console_color_printf_fn = void ( __cdecl * )( void *, const color_t &, const char *, ... );
		return util::get_method< console_color_printf_fn > ( this, 25 )( this, col, format );
	}

	__forceinline void console_printf ( const char *format, ... ) {
		using console_printf_fn = void ( __cdecl * )( void *, const char *, ... );
		return util::get_method< console_printf_fn > ( this, 26 )( this, format );
	}

	__forceinline cvar_t *get_commands ( ) {
		return *reinterpret_cast< cvar_t ** >( reinterpret_cast< uintptr_t >( this ) + 0x30 );
	}

	__forceinline cvar_t *find_var ( const hash32_t hash ) {
		cvar_t *list;

		list = get_commands ( );

		if ( !list )
			return nullptr;

		for ( auto it = list; it != nullptr; it = it->m_next ) {
			if ( HASH ( it->m_name ) == hash ) {
				return it;
			}
		}

		return nullptr;
	}
};