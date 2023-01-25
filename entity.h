#pragma once

enum class movetypes_t : uint8_t {
	none = 0,
	isometric,
	walk,
	step,
	fly,
	flygravity,
	vphysics,
	push,
	noclip,
	ladder,
	observer,
	custom
};

class entity_t {
public:
	template < typename t >
	__forceinline t as ( ) {
		return reinterpret_cast < t > ( this );
	}
	
	POFFSET ( void, renderable, 0x4 );
	POFFSET ( void, networkable, 0x8 );
	OFFSET ( int, idx, 0x64 );
	OFFSET ( float, fall_vel, 0x3004 );
	OFFSET ( int, think_tick, 0xF8 );
	OFFSET ( ucmd_t *, current_cmd, 0x3348 );
	OFFSET ( ucmd_t, last_cmd, 0x3298 );
	OFFSET ( int, buttons, 0x31E8 );
	NETVAR ( int, team, "DT_BaseEntity", "m_iTeamNum" );
	NETVAR ( int, effects, "DT_BaseEntity", "m_fEffects" );
	NETVAR ( vec_t, origin, "DT_BaseEntity", "m_vecOrigin" );
	NETVAR_ADDITIVE ( movetypes_t, movetype, "DT_BaseEntity", "m_nRenderMode", 1 );

	__forceinline bool is_player ( ) {
		using is_player_fn = bool ( __thiscall * )( void * );
		return util::get_method < is_player_fn > ( this, 152 )( this );
	}

	__forceinline bool dormant ( ) {
		using dormant_fn = bool ( __thiscall * )( void * );
		return util::get_method < dormant_fn > ( networkable ( ), 9 )( networkable ( ) );
	}

	__forceinline vec_t &abs_origin ( ) {
		using abs_origin_fn = vec_t & ( __thiscall * )( void * );
		return util::get_method < abs_origin_fn > ( this, 10 )( this );
	}

	__forceinline vec_t &abs_angles ( ) {
		using abs_angles_fn = vec_t & ( __thiscall * )( void * );
		return util::get_method < abs_angles_fn > ( this, 11 )( this );
	}
};