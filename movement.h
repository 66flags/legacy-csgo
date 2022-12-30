#pragma once

struct movement_t {
	void pre_prediction ( ucmd_t *ucmd );
	void run ( ucmd_t *ucmd, const vec_t &old_angs );
	void directional_strafe ( ucmd_t *ucmd, const vec_t &old_angs );
	void correct_movement ( ucmd_t *ucmd, const vec_t & wish_dir );

	vec_t m_strafe_ang;
};

extern movement_t movement;