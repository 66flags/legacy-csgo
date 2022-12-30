#pragma once

struct point_t {
	float x, y;
};

struct rect_t {
	float x, y, w, h;
};

struct dim_t {
	float w, h;
};

struct inputsystem_t {
	std::array < bool, 256 > m_key_state;
	std::array < bool, 256 > m_prev_key_state;

	__forceinline void update ( ) {
		for ( int i = 0; i < 256; i++ ) {
			m_prev_key_state [ i ] = m_key_state [ i ];
			m_key_state [ i ] = GetAsyncKeyState ( i );
		}

		POINT pos;
		GetCursorPos ( &pos );
		ScreenToClient ( GetActiveWindow ( ), &pos );

		m_pos = { static_cast< float >( pos.x ), static_cast< float >( pos.y ) };
	}

	__forceinline bool key_down ( int key ) {
		return m_key_state [ key ];
	}

	__forceinline bool key_pressed ( int key ) {
		return m_key_state [ key ] && !m_prev_key_state [ key ];
	}

	__forceinline bool hovered ( rect_t area ) {
		return m_pos.x > area.x && m_pos.y > area.y && m_pos.x < area.x + area.w && m_pos.y < area.y + area.h;
	}

	__forceinline bool key_released ( int key ) {
		return !m_key_state [ key ] && m_prev_key_state [ key ];
	}

	point_t m_pos;
};

inline inputsystem_t inputsys { };