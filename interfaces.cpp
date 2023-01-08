#include "includes.h"

template < typename t >
__forceinline t create_interface ( const char *module_name, const char *name ) {
	using create_interface_fn = std::uintptr_t *( __cdecl * ) ( const char *, int * );
	const auto create_interface_call = reinterpret_cast< create_interface_fn >( LI_FN ( GetProcAddress )( LI_FN ( GetModuleHandleA )( module_name ), _ ( "CreateInterface" ) ) );

	return reinterpret_cast< t > ( create_interface_call ( name, nullptr ) );
}

bool interfaces::init ( ) {
	m_engine = create_interface < c_engine * > ( _ ( "engine.dll" ), _ ( "VEngineClient014" ) );
	m_surface = create_interface < c_surface * > ( _ ( "vguimatsurface.dll" ), _ ( "VGUI_Surface031" ) );
	m_localize = create_interface < c_localize * > ( _ ( "localize.dll" ), _ ( "Localize_001" ) );
	m_mdl_cache = create_interface < c_model_cache * > ( _ ( "datacache.dll" ), _ ( "MDLCache004" ) );
	m_prediction = create_interface < c_prediction * > ( _ ( "client.dll" ), _ ( "VClientPrediction001" ) );
	m_game_movement = create_interface < c_game_movement * > ( _ ( "client.dll" ), _ ( "GameMovement001" ) );
	m_client = create_interface < c_client * > ( _ ( "client.dll" ), _ ( "VClient018" ) );
	m_trace = create_interface < c_engine_trace * > ( _ ( "engine.dll" ), _ ( "EngineTraceClient004" ) );
	m_entlist = create_interface < c_entlist * > ( _ ( "client.dll" ), _ ( "VClientEntityList003" ) );
	m_cvar = create_interface < c_convar * > ( _ ( "vstdlib.dll" ), _ ( "VEngineCvar007" ) );
	m_event_mgr = create_interface < c_game_event_mgr * > ( _ ( "engine.dll" ), _ ( "GAMEEVENTSMANAGER002" ) );
	m_game_type = create_interface < c_game_type * > ( _ ( "matchmaking.dll" ), _ ( "VENGINE_GAMETYPES_VERSION002" ) );
	m_panel = create_interface < c_panel * > ( _ ( "vgui2.dll" ), _ ( "VGUI_Panel009" ) );
	m_phys_props = create_interface < c_phys * > ( _ ( "vphysics.dll" ), _ ( "VPhysicsSurfaceProps001" ) );
	m_input = pattern::find ( _ ( "client.dll" ), _ ( "B9 ? ? ? ? FF 60 60" ) ).add ( 1 ).deref ( ).as< c_input * > ( );
	m_globals = pattern::find ( _ ( "client.dll" ), _ ( "A1 ? ? ? ? FF 70 04 68 ? ? ? ? 56 E8 ? ? ? ? 8B 06" ) ).add ( 1 ).deref ( ).deref ( ).as < c_global_vars * > ( );
	m_debug_overlay = create_interface< c_debug_overlay * > ( _ ( "engine.dll" ), _ ( "VDebugOverlay004" ) );
	m_device = pattern::find ( _ ( "shaderapidx9.dll" ), _ ( "A1 ? ? ? ? 50 8B 08 FF 51 ? 8B F8" ) ).add ( 1 ).deref ( ).deref ( ).as < IDirect3DDevice9 * > ( );
	m_client_state = pattern::find ( _ ( "engine.dll" ), _ ( "A1 ? ? ? ? 8B 88 ? ? ? ? 85 C9 75 07" ) ).add ( 1 ).deref ( ).deref ( ).as < c_client_state * > ( );
	m_client_mode = pattern::find ( _ ( "client.dll" ), _ ( "8B 0D ? ? ? ? 8B 01 FF 50 1C 85 F6 75 0D A1" ) ).add ( 2 ).deref ( ).deref ( ).as < c_client_mode * > ( );

	return true;
}