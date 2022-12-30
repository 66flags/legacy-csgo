#pragma once

struct gui_t {
	void init ( );
	void draw ( );
	void apply_render_states ( IDirect3DDevice9 *device );
	void restore_render_states ( IDirect3DDevice9 *device );
	void begin ( IDirect3DDevice9 *device, const std::function<void ( )> &fn );

	bool m_visible = true;
	bool m_should_save = false;

	std::deque < std::shared_ptr < ui::window > > m_windows;
	
	__forceinline std::shared_ptr < ui::window > create_window ( const std::string &name, const rect_t &area ) {
		auto window = std::make_shared < ui::window > ( name, area );

		this->m_windows.emplace_back ( window );

		return window;
	}

	__forceinline bool key_toggled ( int key, int toggle ) {
		switch ( toggle ) {
		case 0: {
			return false;
		} break;
		case 1: {
			return true;
		} break;
		case 2: {
			return inputsys.key_down ( key );
		} break;
		case 3: {
			return inputsys.key_pressed ( key );
		} break;
		}

		return false;
	}
};

extern gui_t gui;