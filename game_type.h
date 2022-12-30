#pragma once

enum class game_type_t : int {
	inavlid = -1,
	casual,
	competitive,
	progressive,
	bomb,
	danger_zone = 6,
	max
};

class c_game_type {
	enum indices_t : std::uintptr_t {
		_get_game_type = 8,
		_get_game_mode = 9
	};
public:
	game_type_t get_game_type ( ) {
		using get_game_type_fn = game_type_t ( __thiscall * )( c_game_type * );
		return util::get_method< get_game_type_fn > ( this, indices_t::_get_game_type )( this );
	}

	int get_game_mode ( ) {
		using get_game_mode_fn = int ( __thiscall * )( c_game_type * );
		return util::get_method< get_game_mode_fn > ( this, indices_t::_get_game_mode )( this );
	}
};