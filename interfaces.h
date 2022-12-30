#pragma once

class c_engine_trace;

namespace interfaces {
	inline c_engine *m_engine = nullptr;
	inline c_client *m_client = nullptr;
	inline c_input *m_input = nullptr;
	inline c_entlist *m_entlist = nullptr;
	inline c_surface *m_surface = nullptr;
	inline c_convar *m_cvar = nullptr;
	inline c_global_vars *m_globals = nullptr;
	inline c_localize *m_localize = nullptr;
	inline c_model_cache *m_mdl_cache = nullptr;
	inline c_panel *m_panel = nullptr;
	inline c_game_event_mgr *m_event_mgr = nullptr;
	inline c_game_type *m_game_type = nullptr;
	inline c_debug_overlay *m_debug_overlay = nullptr;
	inline c_move_helper *m_move_helper = nullptr;
	inline c_prediction *m_prediction = nullptr;
	inline c_game_movement *m_game_movement = nullptr;
	inline c_phys *m_phys_props = nullptr;
	inline c_engine_trace *m_trace = nullptr;
	inline c_client_state *m_client_state = nullptr;
	inline IDirect3DDevice9 *m_device = nullptr;
	inline c_client_mode *m_client_mode = nullptr;

	bool init ( );
}