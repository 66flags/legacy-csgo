#pragma once

struct menu_t {
	void init ( );
	void draw ( );
	void apply_render_states ( IDirect3DDevice9 *device );
	void restore_render_states ( IDirect3DDevice9 *device );
	void begin ( IDirect3DDevice9 *device, const std::function < void ( ) > &fn );
	
	bool m_opened = true;
};

extern menu_t menu;