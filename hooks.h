#pragma once

struct detour_t {
	void *m_original { };

	__forceinline bool create ( void *target, void *detour ) {
		/* TODO: debugging features. */
		if ( MH_CreateHook ( target, detour, &m_original ) != MH_OK )
			return false;

		return true;
	}

	template < typename t >
	__forceinline t get_old_method ( ) {
		return reinterpret_cast < t > ( m_original );
	}
};

struct hooks_t {
	bool init ( );
	void remove ( );

	// hooks.
	static bool __fastcall create_move ( void *ecx, void *edx, float input_sample_time, ucmd_t *ucmd );
	static void __fastcall frame_stage_notify ( void *ecx, void *edx, frame_stage_t stage );
	static void __fastcall paint ( void *ecx, void *edx, paint_modes_t mode );
	static LRESULT __stdcall wnd_proc ( HWND hwnd, UINT msg, WPARAM w_param, LPARAM l_param );
	static void __fastcall lock_cursor ( void *ecx, void *edx );
	static HRESULT __stdcall end_scene ( IDirect3DDevice9 *device );
	static HRESULT __stdcall reset ( IDirect3DDevice9 *dev, D3DPRESENT_PARAMETERS *pp );
	static bool __fastcall post_network_data_received ( void *ecx, void *edx, int commands_acknowledged );
	static void __fastcall paint_traverse ( void *ecx, void *edx, unsigned int panel, bool force_repaint, bool allow_force );
	static void __fastcall run_command ( void *ecx, void *edx, player_t *player, ucmd_t *ucmd, c_move_helper *move_helper );;
	static float __fastcall get_alpha_modulation ( void *ecx, void *edx );
	static void __fastcall get_color_modulation ( void *ecx, void *edx, int* r, int* g, int* b );

	// detours.
	detour_t m_create_move;
	detour_t m_frame_stage_notify;
	detour_t m_get_alpha_modulation;
	detour_t m_get_color_modulation;
	detour_t m_paint;
	detour_t m_paint_traverse;
	detour_t m_run_command;
	detour_t m_post_network_data_received;
	detour_t m_end_scene;
	detour_t m_reset;
	detour_t m_lock_cursor;

	// util stuff.
	HWND m_hwnd;
	WNDPROC m_old_wndproc;
};

extern hooks_t hooks;