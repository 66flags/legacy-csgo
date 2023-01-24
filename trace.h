#pragma once

enum contents_t : uint32_t {
	contents_empty = 0,
	contents_solid = 0x1,
	contents_window = 0x2,
	contents_aux = 0x4,
	contents_grate = 0x8,
	contents_slime = 0x10,
	contents_water = 0x20,
	contents_blocklos = 0x40,
	contents_opaque = 0x80,
	last_visible_contents = contents_opaque,
	all_visible_contents = ( last_visible_contents | ( last_visible_contents - 1 ) ),
	contents_testfogvolume = 0x100,
	contents_unused = 0x200,
	contents_blocklight = 0x400,
	contents_team1 = 0x800,
	contents_team2 = 0x1000,
	contents_ignore_nodraw_opaque = 0x2000,
	contents_moveable = 0x4000,
	contents_areaportal = 0x8000,
	contents_playerclip = 0x10000,
	contents_monsterclip = 0x20000,
	contents_current_0 = 0x40000,
	contents_current_90 = 0x80000,
	contents_current_180 = 0x100000,
	contents_current_270 = 0x200000,
	contents_current_up = 0x400000,
	contents_current_down = 0x800000,
	contents_origin = 0x1000000,
	contents_monster = 0x2000000,
	contents_debris = 0x4000000,
	contents_detail = 0x8000000,
	contents_translucent = 0x10000000,
	contents_ladder = 0x20000000,
	contents_hitbox = 0x40000000
};

enum surf_t : uint32_t {
	surf_light = 0x0001,
	surf_sky2d = 0x0002,
	surf_sky = 0x0004,
	surf_warp = 0x0008,
	surf_trans = 0x0010,
	surf_noportal = 0x0020,
	surf_trigger = 0x0040,
	surf_nodraw = 0x0080,
	surf_hint = 0x0100,
	surf_skip = 0x0200,
	surf_nolight = 0x0400,
	surf_bumplight = 0x0800,
	surf_noshadows = 0x1000,
	surf_nodecals = 0x2000,
	surf_nopaint = surf_nodecals,
	surf_nochop = 0x4000,
	surf_hitbox = 0x8000
};

enum mask_t : uint32_t {
	mask_all = ( 0xffffffff ),
	mask_solid = ( contents_solid | contents_moveable | contents_window | contents_monster | contents_grate ),
	mask_playersolid = ( contents_solid | contents_moveable | contents_playerclip | contents_window | contents_monster | contents_grate ),
	mask_npcsolid = ( contents_solid | contents_moveable | contents_monsterclip | contents_window | contents_monster | contents_grate ),
	mask_npcfluid = ( contents_solid | contents_moveable | contents_monsterclip | contents_window | contents_monster ),
	mask_water = ( contents_water | contents_moveable | contents_slime ),
	mask_opaque = ( contents_solid | contents_moveable | contents_opaque ),
	mask_opaque_and_npcs = ( mask_opaque | contents_monster ),
	mask_blocklos = ( contents_solid | contents_moveable | contents_blocklos ),
	mask_blocklos_and_npcs = ( mask_blocklos | contents_monster ),
	mask_visible = ( mask_opaque | contents_ignore_nodraw_opaque ),
	mask_visible_and_npcs = ( mask_opaque_and_npcs | contents_ignore_nodraw_opaque ),
	mask_shot = ( contents_solid | contents_moveable | contents_monster | contents_window | contents_debris | contents_hitbox ),
	mask_shot_brushonly = ( contents_solid | contents_moveable | contents_window | contents_debris ),
	mask_shot_hull = ( contents_solid | contents_moveable | contents_monster | contents_window | contents_debris | contents_grate ),
	mask_shot_portal = ( contents_solid | contents_moveable | contents_window | contents_monster ),
	mask_solid_brushonly = ( contents_solid | contents_moveable | contents_window | contents_grate ),
	mask_playersolid_brushonly = ( contents_solid | contents_moveable | contents_window | contents_playerclip | contents_grate ),
	mask_npcsolid_brushonly = ( contents_solid | contents_moveable | contents_window | contents_monsterclip | contents_grate ),
	mask_npcworldstatic = ( contents_solid | contents_window | contents_monsterclip | contents_grate ),
	mask_npcworldstatic_fluid = ( contents_solid | contents_window | contents_monsterclip ),
	mask_splitareaportal = ( contents_water | contents_slime ),
	mask_current = ( contents_current_0 | contents_current_90 | contents_current_180 | contents_current_270 | contents_current_up | contents_current_down ),
	mask_deadsolid = ( contents_solid | contents_playerclip | contents_window | contents_grate )
};

enum hitgroup_t : int {
	hitgroup_generic = 0,
	hitgroup_head = 1,
	hitgroup_chest = 2,
	hitgroup_stomach = 3,
	hitgroup_leftarm = 4,
	hitgroup_rightarm = 5,
	hitgroup_leftleg = 6,
	hitgroup_rightleg = 7,
	hitgroup_gear = 10
};

enum dispsurf_t {
	dispsurf_flag_surface = ( 1 << 0 ),
	dispsurf_flag_walkable = ( 1 << 1 ),
	dispsurf_flag_buildable = ( 1 << 2 ),
	dispsurf_flag_surfprop1 = ( 1 << 3 ),
	dispsurf_flag_surfprop2 = ( 1 << 4 )
};

class player_t;
class entity_t;

enum trace_type_t {
	trace_everything = 0,
	trace_world_only,
	trace_entities_only,
	trace_everything_filter_props,
};

struct plane_t {
	vec_t m_normal;
	float m_dist;
	std::uint8_t m_type;
	std::uint8_t m_sign_bits;
	PAD ( 2 );
};

class _trace_filter_t {
public:
	virtual bool should_hit_entity ( void *ent, int mask ) = 0;
	virtual trace_type_t get_trace_type ( ) const = 0;
};

class trace_filter_t : public _trace_filter_t {
public:
	trace_filter_t ( ) { };

	trace_filter_t ( void *skip ) {
		this->m_skip = skip;
	}

	virtual trace_type_t get_trace_type ( ) const {
		return trace_everything;
	}

	bool should_hit_entity ( void *ent, int mask ) {
		return !( ent == m_skip );
	}

	void *m_skip = nullptr;
};

class trace_filter_entities_only_t : public trace_filter_t {
public:
	virtual trace_type_t get_trace_type ( ) const {
		return trace_entities_only;
	}
};

class trace_filter_world_only_t : public trace_filter_t {
public:
	bool should_hit_entity ( void *ent, int mask ) {
		return false;
	}

	virtual trace_type_t get_trace_type ( ) const {
		return trace_world_only;
	}
};

class trace_filter_world_and_props_only_t : public trace_filter_t {
public:
	bool should_hit_entity ( void *ent, int mask ) {
		return false;
	}

	virtual trace_type_t get_trace_type ( ) const {
		return trace_everything;
	}
};

class trace_filter_hit_all_t : public trace_filter_t {
public:
	bool should_hit_entity ( void *ent, int mask ) {
		return false;
	}
};

using should_hit_func_t = bool ( * )( void *, int );
static auto trace_filter_simple_vmt = pattern::find ( _ ( "client.dll" ), _ ( "55 8B EC 83 E4 F0 83 EC 7C 56 52" ) ).add ( 61 ).deref ( ).as < void * > ( );

class trace_filter_simple_game_t {
public:
	void *m_vmt;
	const entity_t *m_pass_ent1;
	int m_collision_group;
	should_hit_func_t m_shouldhit_check_fn;
public:
	trace_filter_simple_game_t ( ) : m_vmt { trace_filter_simple_vmt }, m_pass_ent1 { } , m_collision_group { } , m_shouldhit_check_fn { } { }
	trace_filter_simple_game_t ( const void *pass_ent1, int collision_group = 0, should_hit_func_t shouldhit_check_fn = nullptr ) : m_vmt { trace_filter_simple_vmt }, m_pass_ent1 { static_cast < const entity_t* > ( pass_ent1 ) }, m_collision_group { collision_group }, m_shouldhit_check_fn { shouldhit_check_fn } {}

	bool should_hit_entity ( void *ent, int mask ) {
		void *real_vmt = *reinterpret_cast < void ** > ( m_vmt );
		return util::get_method < bool ( __thiscall * )( void *, void *, int ) > ( real_vmt, 0 ) ( real_vmt, ent, mask );
	}

	void set_pass_entity ( entity_t *pass_ent1 ) {
		m_pass_ent1 = pass_ent1;
	}

	void set_collision_group ( int collision_group ) {
		m_collision_group = collision_group;
	}
};

struct ray_t {
	vec_aligned_t start;
	vec_aligned_t delta;
	vec_aligned_t start_offset;
	vec_aligned_t extents;
	const matrix3x4_t *world_axis_transform;
	bool is_ray;
	bool is_swept;

	ray_t ( ) : world_axis_transform ( NULL ) { }

	void init ( vec_t const &src, vec_t const &end ) {
		delta = end - src;
		is_swept = delta.length_sqr ( ) != 0.f;
		extents.init ( );
		world_axis_transform = NULL;
		is_ray = true;
		start_offset.init ( );
		start = src;
	}

	void init ( vec_t const &m_start, vec_t const &end, vec_t const &mins, vec_t const &maxs ) {
		delta = end - m_start;
		world_axis_transform = nullptr;
		is_swept = delta.length_sqr ( ) != 0.0f;

		extents = maxs - mins;
		extents *= 0.5f;
		is_ray = extents.length_sqr ( ) < 1e-6f;

		start_offset = maxs + mins;
		start_offset *= 0.5f;
		start = m_start + start_offset;
		start_offset *= -1.0f;
	}
};

struct surface_t {
	const char *m_name;
	short m_props;
	std::uint8_t m_flags;
};

class base_trace_t {
public:
	bool is_disp_surface ( void ) { return ( ( m_flags & dispsurf_flag_surface ) != 0 ); }
	bool is_disp_surface_walkable ( void ) { return ( ( m_flags & dispsurf_flag_walkable ) != 0 ); }
	bool is_disp_surface_buildable ( void ) { return ( ( m_flags & dispsurf_flag_buildable ) != 0 ); }
	bool is_disp_surface_prop1 ( void ) { return ( ( m_flags & dispsurf_flag_surfprop1 ) != 0 ); }
	bool is_disp_surface_prop2 ( void ) { return ( ( m_flags & dispsurf_flag_surfprop2 ) != 0 ); }
public:
	vec_t		    m_startpos; // 0x0000
	vec_t		    m_endpos; // 0x0010
	plane_t		    m_plane; // 0x0020
	float			m_fraction; // 0x0030
	uint32_t 		m_contents; // 0x0034
	uint16_t	    m_flags; // 0x0038
	bool			m_allsolid; // 0x003A
	bool			m_startsolid; // 0x003B

	base_trace_t ( ) { }
};

class trace_t : public base_trace_t {
public:
	__forceinline bool did_hit_world ( ) const {
		return m_hit_entity == interfaces::m_entlist->get< void * > ( 0 );
	}

	__forceinline bool did_hit_non_world_ent ( ) const {
		return m_hit_entity != nullptr && !did_hit_world ( );
	}

	__forceinline bool did_hit ( ) const {
		return m_fraction < 1 || m_allsolid || m_startsolid;
	}

	float m_fractionleftsolid; // 0x003C
	surface_t m_surface; // 0x0040
	int m_hitgroup; // 0x0048
	short m_physicsbone; // 0x004C
	uint16_t m_world_surface_index; // 0x004E
	entity_t *m_hit_entity; // 0x0050
	int m_hitbox; // 0x0054

	trace_t ( ) : m_hit_entity ( nullptr ) { }

	trace_t ( const trace_t &other ) :
		m_fractionleftsolid ( other.m_fractionleftsolid ),
		m_surface ( other.m_surface ),
		m_hitgroup ( other.m_hitgroup ),
		m_physicsbone ( other.m_physicsbone ),
		m_world_surface_index ( other.m_world_surface_index ),
		m_hit_entity ( other.m_hit_entity ),
		m_hitbox ( other.m_hitbox ) {
		m_startpos = other.m_startpos;
		m_endpos = other.m_endpos;
		m_plane = other.m_plane;
		m_fraction = other.m_fraction;
		m_contents = other.m_contents;
		m_flags = other.m_flags;
		m_allsolid = other.m_allsolid;
		m_startsolid = other.m_startsolid;
	}
};

class c_engine_trace {
public:
	virtual int get_point_contents ( const vec_t &origin, int mask = mask_all, void **ent = nullptr ) = 0;
	virtual void *pad001 ( ) = 0;
	virtual void *pad002 ( ) = 0;
	virtual void clip_ray_to_entity ( const ray_t &_ray, unsigned mask, void *ent, trace_t *_trace ) = 0;
	virtual void clip_ray_to_collideable ( const ray_t &_ray, unsigned int mask, void *collide, trace_t *_trace ) = 0;
	virtual void trace_ray ( const ray_t &_ray, unsigned int mask, trace_filter_t *filter, trace_t *_trace ) = 0;
};