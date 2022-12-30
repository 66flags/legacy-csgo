#pragma once

enum option_type_t {
	option_int,
	option_float,
	option_bool,
	option_string,
	option_color
};

struct option_t {
	option_type_t type { };
	std::string name { };

	int i;
	float f;
	bool b;
	std::string s;
	color_t c = { 255, 255, 255, 255 };

	static void add_int ( const std::string &name, int value );
	static void add_float ( const std::string &name, float value );
	static void add_bool ( const std::string &name, bool value );
	static void add_string ( const std::string &name, const std::string &value );
	static void add_color ( const std::string &name, const color_t &value );
};

inline std::unordered_map < std::string, option_t > options { };

struct settings_t {
	void init ( );
	void save ( const std::string &name );
	bool load ( const std::string &name );
public:
	template < typename t >
	inline t &get_item ( const std::string &name ) {
		switch ( options [ name ].type ) {
		case option_type_t::option_color: {
			return ( t & ) options [ name ].c;
		} break;
		case option_type_t::option_int: {
			return ( t & ) options [ name ].i;
		} break;
		case option_type_t::option_float: {
			return ( t & ) options [ name ].f;
		} break;
		case option_type_t::option_bool: {
			return ( t & ) options [ name ].b;
		} break;
		case option_type_t::option_string: {
			return ( t & ) options [ name ].s;
		} break;
		}
	}
};

extern settings_t settings;