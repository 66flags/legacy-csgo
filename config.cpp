#include "includes.h"

settings_t settings { };

void settings_t::init ( ) {
	option_t::add_bool ( _ ( "misc.unlimited_duck" ), false );
	option_t::add_bool ( _ ( "visuals.transparent_props" ), false );
    option_t::add_int ( _ ( "transparent_props_amount" ), 0 );
    option_t::add_bool ( _ ( "misc.bunny_hop" ), false );
    option_t::add_bool ( _ ( "misc.auto_strafe" ), false );
    option_t::add_int ( _ ( "misc.auto_strafe_type" ), 1 );
	option_t::add_bool ( _ ( "visuals.modulate_world" ), false );
    option_t::add_bool ( _ ( "visuals.thirdperson" ), false );
	option_t::add_int ( _ ( "visuals.thirdperson_distance" ), 160 );
    option_t::add_int ( _ ( "visuals.thirdperson_key" ), 0 );
	option_t::add_int ( _ ( "visuals.thirdperson_key_toggle" ), 2 );
	option_t::add_color ( _ ( "visuals.modulate_world_color" ), sesui::color ( 1.0f, 1.0f, 1.0f, 1.0f ) );
}

void option_t::add_int ( const std::string &name, int value ) {
    options [ name ].type = option_type_t::option_int;
    options [ name ].i = value;
}

void option_t::add_float ( const std::string &name, float value ) {
    options [ name ].type = option_type_t::option_float;
    options [ name ].f = value;
}

void option_t::add_bool ( const std::string &name, bool value ) {
    options [ name ].type = option_type_t::option_bool;
    options [ name ].b = value;
}

void option_t::add_string ( const std::string &name, const std::string &value ) {
    options [ name ].type = option_type_t::option_string;
    options [ name ].s = value;
}

void option_t::add_color ( const std::string &name, const sesui::color &value ) {
    options [ name ].type = option_type_t::option_color;
    options [ name ].c = value;
}

void settings_t::save ( const std::string &name ) {
    using namespace nlohmann;

    json parsed_data { };

    for ( auto &option : options ) {
        switch ( option.second.type ) {
        case option_type_t::option_color: {
            parsed_data [ option.first ][ 0 ] = option.second.c.r;
            parsed_data [ option.first ][ 1 ] = option.second.c.g;
            parsed_data [ option.first ][ 2 ] = option.second.c.b;
            parsed_data [ option.first ][ 3 ] = option.second.c.a;
        } break;
        case option_type_t::option_int: {
            parsed_data [ option.first ] = option.second.i;
        } break;
        case option_type_t::option_float: {
            parsed_data [ option.first ] = option.second.f;
        } break;
        case option_type_t::option_bool: {
            parsed_data [ option.first ] = option.second.b;
        } break;
        case option_type_t::option_string: {
            parsed_data [ option.first ] = option.second.s;
        } break;
        }
    }

    std::ofstream file { name + _ ( ".json" ) };

    file << std::setw ( 4 ) << parsed_data << std::endl;

    file.close ( );
}

bool settings_t::load ( const std::string &name ) {
    using namespace nlohmann;

    std::ifstream file { name + _ ( ".json" ) };

    if ( !file.is_open ( ) ) {
        return false;
    }

    json parsed_data { };

    file >> parsed_data;

    for ( auto &option : options ) {
        switch ( option.second.type ) {
        case option_type_t::option_color: {
            option.second.c.r = parsed_data [ option.first ][ 0 ];
            option.second.c.g = parsed_data [ option.first ][ 1 ];
            option.second.c.b = parsed_data [ option.first ][ 2 ];
            option.second.c.a = parsed_data [ option.first ][ 3 ];
        } break;
        case option_type_t::option_int: {
            option.second.i = parsed_data [ option.first ];
        } break;
        case option_type_t::option_float: {
            option.second.f = parsed_data [ option.first ];
        } break;
        case option_type_t::option_bool: {
            option.second.b = parsed_data [ option.first ];
        } break;
        case option_type_t::option_string: {
            option.second.s = parsed_data [ option.first ];
        } break;
        }
    }

    return true;
}

void settings_t::refresh ( ) {
    auto sanitize_str = [ ] ( const std::wstring &text ) -> std::wstring {
		std::string str = std::string ( text.begin ( ), text.end ( ) );

        size_t last_index = str.find_last_of ( "." );
        std::string raw = str.substr ( 0, last_index );

		return std::wstring ( raw.begin ( ), raw.end ( ) );
    };

    for ( const auto &p : std::filesystem::directory_iterator ( std::filesystem::current_path ( ) ) ) {
		if ( p.is_regular_file ( ) && p.exists ( ) && p.path ( ).extension ( ) == _ ( ".json" ) )
            config_list.push_back ( sanitize_str ( p.path ( ).filename ( ).wstring ( ) ) );
    }
}