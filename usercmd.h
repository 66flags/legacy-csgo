#pragma once

enum buttons_t : int {
	attack = ( 1 << 0 ),
	jump = ( 1 << 1 ),
	duck = ( 1 << 2 ),
	forward = ( 1 << 3 ),
	back = ( 1 << 4 ),
	use = ( 1 << 5 ),
	cancel = ( 1 << 6 ),
	left = ( 1 << 7 ),
	right = ( 1 << 8 ),
	move_left = ( 1 << 9 ),
	move_right = ( 1 << 10 ),
	attack2 = ( 1 << 11 ),
	run = ( 1 << 12 ),
	reload = ( 1 << 13 ),
	alt1 = ( 1 << 14 ),
	alt2 = ( 1 << 15 ),
	score = ( 1 << 16 ),
	speed = ( 1 << 17 ),
	walk = ( 1 << 18 ),
	zoom = ( 1 << 19 ),
	weapon1 = ( 1 << 20 ),
	weapon2 = ( 1 << 21 ),
	bullrush = ( 1 << 22 ),
};

class ucmd_t {
public:
	virtual       ~ucmd_t ( ) { };
	int           m_command_number;
	int           m_tick_count;
	vec_t         m_ang;
	vec_t         m_aimdir;
	float         m_fmove;
	float         m_smove;
	float         m_umove;
	int           m_buttons;
	unsigned char m_impulse;
	int           m_weaponselect;
	int           m_weaponsubtype;
	int           m_random_seed;
	short         m_mousedx;
	short         m_mousedy;
	bool          m_hasbeenpredicted;
	vec_t		  m_head_angles;
	vec_t		  m_head_offset;

	uint32_t get_checksum ( ) {
		uint32_t crc;
		crc32::init ( crc );

		crc32::process ( crc, m_command_number );
		crc32::process ( crc, m_tick_count );
		crc32::process ( crc, m_ang );
		crc32::process ( crc, m_aimdir );
		crc32::process ( crc, m_fmove );
		crc32::process ( crc, m_smove );
		crc32::process ( crc, m_umove );
		crc32::process ( crc, m_buttons );
		crc32::process ( crc, m_impulse );
		crc32::process ( crc, m_weaponselect );
		crc32::process ( crc, m_weaponsubtype );
		crc32::process ( crc, m_random_seed );
		crc32::process ( crc, m_mousedx );
		crc32::process ( crc, m_mousedy );

		crc32::final ( crc );

		return crc;
	}
};

class verified_ucmd_t {
public:
	ucmd_t        m_cmd;
	unsigned long m_crc;
};