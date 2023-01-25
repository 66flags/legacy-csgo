#pragma once

struct box_t {
	int x, y, w, h;
};

struct visuals_t {
	void paint ( );
	void player ( player_t *pl );
	void draw ( entity_t *ent );
	bool get_player_box ( player_t *ent, box_t &box );
};

extern visuals_t visuals;