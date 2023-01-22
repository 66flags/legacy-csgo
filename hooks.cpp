#include "includes.h"

hooks_t hooks { };

std::unique_ptr < c_event_handler > event_handler = nullptr;

void set_cvar_values ( ) {
	static cvar_t *cl_extrapolate = interfaces::m_cvar->find_var ( HASH ( "cl_extrapolate" ) );

	cl_extrapolate->remove_callbacks ( );
	cl_extrapolate->set_int ( 0 );

	static cvar_t *cl_csm_shadows = interfaces::m_cvar->find_var ( HASH ( "cl_csm_shadows" ) );

	cl_csm_shadows->remove_callbacks ( );
	cl_csm_shadows->set_int ( 0 );

	static cvar_t *cl_foot_contact_shadows = interfaces::m_cvar->find_var ( HASH ( "cl_foot_contact_shadows" ) );

	cl_foot_contact_shadows->remove_callbacks ( );
	cl_foot_contact_shadows->set_int ( 0 );

	static cvar_t *name = interfaces::m_cvar->find_var ( HASH ( "name" ) );
	name->remove_callbacks ( );

	static cvar_t *r_DrawSpecificStaticProp = interfaces::m_cvar->find_var ( HASH ( "r_DrawSpecificStaticProp" ) );
	r_DrawSpecificStaticProp->remove_callbacks ( );

	interfaces::m_engine->client_cmd_unrestricted ( _ ( "rate 786432" ) );
}

bool hooks_t::init ( ) {
	set_cvar_values ( );

	m_hwnd = LI_FN ( FindWindowA )( _ ( "Valve001" ), nullptr );
	m_old_wndproc = reinterpret_cast < WNDPROC > ( SetWindowLongPtrA ( m_hwnd, GWLP_WNDPROC, LONG_PTR ( wnd_proc ) ) );

	if ( MH_Initialize ( ) != MH_OK )
		throw std::runtime_error ( _ ( "Failed to initialize minhook." ) );

	menu.init ( );
	settings.refresh ( );

	const auto _create_move = util::get_method < void * > ( interfaces::m_client_mode, 24 );
	const auto _frame_stage_notify = util::get_method < void * > ( interfaces::m_client, 36 );
	const auto _paint = pattern::find ( _ ( "engine.dll" ), _ ( "55 8B EC 83 EC 40 53 8B D9 8B 0D ? ? ? ? 89" ) ).as< void * > ( );
	const auto _get_alpha_modulation = pattern::find ( _ ( "materialsystem.dll" ), _ ( "55 8B EC 83 EC 0C 56 8B F1 8A 46 20 C0 E8 02 A8" ) ).sub ( 48 ).as < void * > ( );
	const auto _paint_traverse = util::get_method < void * > ( interfaces::m_panel, 41 );
	const auto _run_command = util::get_method < void * > ( interfaces::m_prediction, 19 );
	const auto _end_scene = util::get_method < void * > ( interfaces::m_device, 42 );
	const auto _reset = util::get_method < void * > ( interfaces::m_device, 16 );
	const auto _post_network_data_received = pattern::find ( _ ( "client.dll" ), _ ( "55 8B EC 83 E4 ? 83 EC ? 53 56 57 6A ? 6A" ) ).as < void * > ( );
	const auto _lock_cursor = util::get_method < void * > ( interfaces::m_surface, 67 );

	m_create_move.create ( _create_move, create_move );
	m_frame_stage_notify.create ( _frame_stage_notify, frame_stage_notify );
	m_paint.create ( _paint, paint );
	m_paint_traverse.create ( _paint_traverse, paint_traverse );
	m_run_command.create ( _run_command, run_command );
	m_end_scene.create ( _end_scene, end_scene );
	m_reset.create ( _reset, reset );
	m_lock_cursor.create ( _lock_cursor, lock_cursor );
	m_post_network_data_received.create ( _post_network_data_received, post_network_data_received );
	m_get_alpha_modulation.create ( _get_alpha_modulation, get_alpha_modulation );
	//m_get_color_modulation.create ( _get_color_modulation, get_color_modulation );

	/* init event handler. */
	event_handler = std::make_unique < c_event_handler > ( );

	MH_EnableHook ( MH_ALL_HOOKS );

	return true;
}

void hooks_t::remove ( ) {
	SetWindowLongPtrA ( m_hwnd, GWLP_WNDPROC, LONG_PTR ( m_old_wndproc ) );

	MH_DisableHook ( MH_ALL_HOOKS );
	MH_RemoveHook ( MH_ALL_HOOKS );
	MH_Uninitialize ( );
}