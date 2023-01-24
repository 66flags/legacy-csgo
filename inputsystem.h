#pragma once
#include <array>
#include <windows.h>

namespace input {
    inline std::array < bool, 256 > key_state;
    inline std::array < bool, 256 > prev_key_state;

    __forceinline void update_input ( ) {
        for ( int i = 0; i < 256; i++ ) {
            prev_key_state [ i ] = key_state [ i ];
            key_state [ i ] = GetAsyncKeyState ( i );
        }
    }

    __forceinline bool key_pressed ( int key ) {
        return key_state [ key ] && !prev_key_state [ key ];
    }

    __forceinline bool key_down ( int key ) {
        return key_state [ key ];
    }

    __forceinline bool key_released ( int key ) {
        return !key_state [ key ] && prev_key_state [ key ];
    }

	inline std::unordered_map < std::string, bool > hotkey_states;

	__forceinline bool keybind_active ( const std::string& name, int vkey, int mode ) {
		if ( hotkey_states.find ( name ) == hotkey_states.end ( ) )
			hotkey_states [ name ] = sesui::input::key_down ( vkey );

		switch ( mode ) {
		case 0:
			hotkey_states [ name ] = false;
			break;
		case 1:
			hotkey_states [ name ] = key_state [ vkey ];
			break;
		case 2:
			if ( key_state [ vkey ] && !prev_key_state [ vkey ] )
				hotkey_states [ name ] = !hotkey_states [ name ];
			break;
		case 3:
			hotkey_states [ name ] = true;
			break;
		}

		return hotkey_states [ name ];
	}

}