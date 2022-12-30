#include "includes.h"

settings_t settings { };

void settings_t::init ( ) {
	option_t::add_bool ( _ ( "misc.bunny_hop" ), false );
	option_t::add_bool ( _ ( "misc.auto_strafe" ), false );
	option_t::add_int ( _ ( "misc.auto_strafe_type" ), 1 );
	option_t::add_bool ( _ ( "misc.unlimited_duck" ), false );
	option_t::add_bool ( _ ( "visuals.grenade_visualization" ), false );
	option_t::add_color ( _ ( "visuals.grenade_line_color" ), { 255, 166, 166 } );
	option_t::add_color ( _ ( "visuals.grenade_circle_color" ), { 166, 215, 255 } );
	option_t::add_bool ( _ ( "visuals.prop_transparency" ), false );
    option_t::add_float ( _ ( "visuals.prop_transparency_amt" ), 100.0f );
	option_t::add_float ( _ ( "visuals.thirdperson_dist" ), 120.0f );
    option_t::add_bool ( _ ( "events.hurt" ), true );
	option_t::add_bool ( _ ( "events.buy" ), false );
	option_t::add_bool ( _ ( "events.bomb" ), true );
	option_t::add_bool ( _ ( "events.round" ), false );
    option_t::add_bool ( _ ( "visuals.thirdperson" ), false );
    option_t::add_int ( _ ( "visuals.thirdperson_key" ), 0 );
    option_t::add_int ( _ ( "visuals.thirdperson_toggle" ), 0 );
	option_t::add_bool ( _ ( "visuals.thirdperson_spectator" ), false );

	option_t::add_int ( _ ( "visuals.visual_toggle_key" ), 0 );
	option_t::add_int ( _ ( "visuals.visual_toggle_key_toggle" ), 1 );
	option_t::add_bool ( _ ( "visuals.players_enable" ), false );
	option_t::add_bool ( _ ( "visuals.dormant" ), false );
	option_t::add_float ( _ ( "visuals.dormancy_time" ), 0.0f );
	option_t::add_color ( _ ( "visuals.health_color1" ), { 0, 255, 0 } );
	option_t::add_color ( _ ( "visuals.health_color2" ), { 255, 0, 0 } );
	option_t::add_bool ( _ ( "visuals.health" ), false );
	option_t::add_color ( _ ( "visuals.name_color" ), { 255, 255, 255 } );
	option_t::add_bool ( _ ( "visuals.name" ), false );
	option_t::add_color ( _ ( "visuals.weapon_color" ), { 255, 255, 255 } );
	option_t::add_bool ( _ ( "visuals.weapon" ), false );
	option_t::add_color ( _ ( "visuals.box_color" ), { 255, 255, 255 } );
	option_t::add_bool ( _ ( "visuals.box" ), false );
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

void option_t::add_color ( const std::string &name, const color_t &value ) {
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

    std::ofstream file { name };

    file << std::setw ( 4 ) << parsed_data << std::endl;

    file.close ( );
}

bool settings_t::load ( const std::string &name ) {
    using namespace nlohmann;

    std::ifstream file { name };

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