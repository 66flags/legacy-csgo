#pragma once

struct globals_t {
	frame_stage_t m_stage;
	HMODULE m_module;
	player_t *m_local = nullptr;
	ucmd_t *m_cmd = nullptr;
	bool m_thirdperson = false;
}; inline globals_t g;