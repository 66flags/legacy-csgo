#pragma once

struct surfacedata_t;

struct surfacephysicsparams_t {
    PAD ( 20 );
};

struct surfaceaudioparams_t {
    PAD ( 36 );
};

struct surfacesoundnames_t {
    PAD ( 24 );
};

struct surfacegameprops_t {
    PAD ( 8 );
    float m_penetration_modifier;
    float m_damage_modifier;
    unsigned short m_material;
    unsigned char m_climbable;
    PAD ( 4 );
};

struct surfacedata_t {
    surfacephysicsparams_t m_physics;	  
    surfaceaudioparams_t m_audio;
    surfacesoundnames_t	m_sounds;
    surfacegameprops_t m_game;
    PAD ( 48 );
};

class c_phys {
    enum indices_t : std::uintptr_t {
		_get_surface_data = 5,
    };
public:
    surfacedata_t *get_surface_data ( int idx ) {
        using o_fn = surfacedata_t * ( __thiscall * )( void *, int );
        return util::get_method < o_fn > ( this, indices_t::_get_surface_data )( this, idx );
    }
};