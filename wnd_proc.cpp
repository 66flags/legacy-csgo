#include "includes.h"

LRESULT __stdcall hooks_t::wnd_proc ( HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam ) {
	bool allow_input_processing = false;

	if ( menu.m_opened ) {
		allow_input_processing = true;

		switch ( msg ) {
		case WM_MOUSEWHEEL: {
			sesui::input::scroll_amount += static_cast< float > ( GET_WHEEL_DELTA_WPARAM ( wparam ) ) / static_cast< float > ( WHEEL_DELTA );
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