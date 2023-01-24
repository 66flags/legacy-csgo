#pragma once

class c_input {
	enum indices_t : std::uintptr_t {
		_cam_tothirdperson = 35,
		_cam_tofirstperson = 36,
	};
public:
	__forceinline void cam_to_thirdperson ( ) {
		using cam_to_thirdperson_fn = void ( __thiscall * ) ( void * );
		util::get_method < cam_to_thirdperson_fn > ( this, indices_t::_cam_tothirdperson ) ( this );
	}

	__forceinline void cam_to_firstperson ( ) {
		using cam_to_firstperson_fn = void ( __thiscall * ) ( void * );
		util::get_method < cam_to_firstperson_fn > ( this, indices_t::_cam_tofirstperson ) ( this );
	}

	void *vftable; 
	PAD ( 160 ); 
	bool m_camera_intercepting_mouse; 
	bool m_camera_in_thirdperson; 
	PAD ( 2 ); 
	vec_t m_camera_offset; 
	PAD ( 56 ); 
	ucmd_t *m_cmds; 
	verified_ucmd_t *m_verified_cmds; 
	PAD ( 84 ); 

	__forceinline ucmd_t *get_cmd ( int sequence_number ) {
		return &m_cmds [ sequence_number % 150 ];
	}

	__forceinline verified_ucmd_t *get_verified_cmd ( int sequence_number ) {
		return &m_verified_cmds [ sequence_number % 150 ];
	}
};