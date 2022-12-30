#pragma once

struct globals_t {
	frame_stage_t m_stage;
	HMODULE m_module;
	player_t *m_local = nullptr;
	ucmd_t *m_cmd = nullptr;
	std::string m_cfg_name = _ ( "config_default_hypnotic.json" );
}; inline globals_t g { };