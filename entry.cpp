#include "includes.h"

void __stdcall cheat_thread ( ) {
	while ( !LI_FN ( GetModuleHandleA )( _ ( "serverbrowser.dll" ) ) )
		std::this_thread::sleep_for ( std::chrono::milliseconds ( 400 ) );

	/* init interfaces. */
	interfaces::init ( );

	/* init others. */
	netvars.init ( );
	render.init ( );
	hooks.init ( );

	/* notify our user that we successfully injected. */
	notify.add ( _ ( "Injected successfully.\n" ) );

	/* unload our hack. */
	while ( !LI_FN ( GetAsyncKeyState )( VK_END ) )
		std::this_thread::sleep_for ( std::chrono::milliseconds ( 200 ) );

	hooks.remove ( );
	LI_FN ( FreeLibraryAndExitThread )( g.m_module, 0 );
}

int __stdcall DllMain ( HMODULE mod, unsigned long reason, void *reserved ) {
	g.m_module = mod;

	if ( reason == DLL_PROCESS_ATTACH ) {
		const auto handle = LI_FN ( CreateThread )( nullptr, 0, LPTHREAD_START_ROUTINE ( cheat_thread ), mod, 0, nullptr );

		if ( !handle )
			return 0;

		LI_FN ( CloseHandle )( handle );
	}

	return 1;
}