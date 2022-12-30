#pragma once

class c_input {
public:
	void *vftable;
	PAD ( 0x8 );
	bool m_track_ir_available;
	bool m_mouse_initialized;
	bool m_mouse_active;
	PAD ( 0x9A );
	bool m_camera_in_thirdperson;
	PAD ( 0x2 );
	vec_t m_camera_offset;
	PAD ( 0x38 );
	ucmd_t *m_cmds;
	verified_ucmd_t *m_verified_cmds;

	ucmd_t *get_cmd ( int sequence_number ) {
		return &m_cmds [ sequence_number % 150 ];
	}

	verified_ucmd_t *get_verified_cmd ( int sequence_number ) {
		return &m_verified_cmds [ sequence_number % 150 ];
	}
};