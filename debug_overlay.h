#pragma once

class c_debug_overlay {
public:
	virtual void add_entity_text_overlay ( int idx, int offset, float duration, int r, int g, int b, int a, const char *format, ... ) = 0;
	virtual void add_box_overlay ( const vec_t &origin, const vec_t &mins, const vec_t &max, vec_t const &orientation, int r, int g, int b, int a, float duration ) = 0;
	virtual void add_sphere_overlay ( const vec_t &origin, float rad, int theta, int phi, int r, int g, int b, int a, float duration ) = 0;
	virtual void add_triangle_overlay ( const vec_t &p1, const vec_t &p2, const vec_t &p3, int r, int g, int b, int a, bool no_depth_test, float duration ) = 0;
	virtual void pad001 ( ) = 0;
	virtual void add_line_overlay ( const vec_t &origin, const vec_t &dest, int r, int g, int b, bool no_depth_test, float duration ) = 0;
};