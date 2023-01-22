#pragma once

struct material_t {
	enum indices_t : std::uintptr_t {
		_get_name = 0,
		_get_texture_group_name = 1,
		_increment_reference_count = 12,
		_decrement_reference_count = 13,
		_alpha_modulate = 27,
		_color_modulate = 28,
		_is_error_material = 42,
	};
public:
	__forceinline const char *get_name ( ) {
		using get_name_fn = const char *( __thiscall * )( void * );
		return util::get_method< get_name_fn > ( this, indices_t::_get_name )( this );
	}
	
	__forceinline const char *get_texture_group_name ( ) {
		using get_texture_group_name_fn = const char *( __thiscall * )( void * );
		return util::get_method < get_texture_group_name_fn > ( this, indices_t::_get_texture_group_name )( this );
	}

	__forceinline void increment_reference_count ( ) {
		using increment_reference_count_fn = void ( __thiscall * )( void * );
		return util::get_method < increment_reference_count_fn > ( this, indices_t::_increment_reference_count )( this );
	}

	__forceinline void decrement_reference_count ( ) {
		using decrement_reference_count_fn = void ( __thiscall * )( void * );
		return util::get_method < decrement_reference_count_fn > ( this, indices_t::_decrement_reference_count )( this );
	}

	__forceinline void alpha_modulate ( float alpha ) {
		using alpha_modulate_fn = void ( __thiscall * )( void *, float );
		return util::get_method < alpha_modulate_fn > ( this, indices_t::_alpha_modulate )( this, alpha );
	}

	__forceinline void color_modulate ( int r, int g, int b ) {
		using color_modulate_fn = void ( __thiscall * )( void *, float, float, float );
		return util::get_method < color_modulate_fn > ( this, indices_t::_color_modulate )( this, static_cast < float > ( r ) / 255.0f, static_cast < float > ( g ) / 255.0f, static_cast < float > ( b ) / 255.0f );
	}

	__forceinline bool is_error_material ( ) {
		using is_error_material_fn = bool ( __thiscall * )( void * );
		return util::get_method < is_error_material_fn > ( this, indices_t::_is_error_material )( this );
	}
};

class c_material_system {
	enum indices_t : std::uintptr_t {
		_find_material = 84,
		_first_material = 86,
		_next_material = 87,
		_invalid_material = 88,
		_get_material = 89
	};
public:
	__forceinline uint16_t first_material ( ) {
		using first_material_fn = uint16_t ( __thiscall * )( void * );
		return util::get_method < first_material_fn > ( this, indices_t::_first_material )( this );
	}

	__forceinline uint16_t next_material ( uint16_t handle ) {
		using next_material_fn = uint16_t ( __thiscall * )( void *, uint16_t );
		return util::get_method < next_material_fn > ( this, indices_t::_next_material )( this, handle );
	}

	__forceinline uint16_t invalid_material ( ) {
		using invalid_material_fn = uint16_t ( __thiscall * )( void * );
		return util::get_method < invalid_material_fn > ( this, indices_t::_invalid_material )( this );
	}

	__forceinline material_t *get_material ( uint16_t handle ) {
		using get_material_fn = material_t * ( __thiscall * )( void *, uint16_t );
		return util::get_method < get_material_fn > ( this, indices_t::_get_material )( this, handle );
	}

	__forceinline material_t *find_material ( const char *name, char *type ) {
		using find_material_fn = material_t * ( __thiscall * )( void *, const char *, char *, bool, void * );
		return util::get_method < find_material_fn > ( this, indices_t::_find_material )( this, name, type, 1, 0 );
	}
};