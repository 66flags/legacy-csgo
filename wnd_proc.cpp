#include "includes.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler ( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

LRESULT __stdcall hooks_t::wnd_proc ( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam ) {
	if ( !ui::wndproc ( hwnd, msg, wparam, lparam ) )
		return true;

	bool allow_input_processing = false;

	if ( gui.m_visible ) {
		allow_input_processing = true;

		switch ( msg ) {
		case WM_MOUSEWHEEL: {
		} break;
		case WM_KEYDOWN: {
			if ( LOWORD ( wparam ) == VK_SPACE || LOWORD ( wparam ) == 0x57 || LOWORD ( wparam ) == 0x41 || LOWORD ( wparam ) == 0x53 || LOWORD ( wparam ) == 0x44 )
				allow_input_processing = false;
		} break;
		case WM_KEYUP: {
			if ( LOWORD ( wparam ) == VK_SPACE || LOWORD ( wparam ) == 0x57 || LOWORD ( wparam ) == 0x41 || LOWORD ( wparam ) == 0x53 || LOWORD ( wparam ) == 0x44 )
				allow_input_processing = false;
		} break;
		}
	}
	else
		allow_input_processing = false;

	if ( allow_input_processing )
		return true;

	return ::CallWindowProcA ( hooks.m_old_wndproc, hwnd, msg, wparam, lparam );
}