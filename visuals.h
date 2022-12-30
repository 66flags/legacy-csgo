#pragma once

struct box_t {
	int x, y, w, h;
};

struct visuals_t {
	void paint ( );
	bool get_box_bounds ( player_t *ent, box_t &box );
};

extern visuals_t visuals;