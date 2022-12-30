#pragma once

namespace math {
	constexpr float pi = 3.14159265358979f;

	__forceinline float rad2deg ( float rad ) {
		float res = rad * ( 180.0f / pi );
		return res;
	}

	__forceinline float deg2rad ( float deg ) {
		float res = deg * ( pi / 180.0f );
		return res;
	}

	vec_t angle_vector ( vec_t angle );
	void angle_vectors ( const vec_t &angle, vec_t *forward = nullptr, vec_t *right = nullptr, vec_t *up = nullptr );
	void normalize ( vec_t &ang );
	vec_t vector_angle ( vec_t vector );
	void sin_cos ( float radians, float *sin, float *cos );
	void clamp ( vec_t &ang );
	float dot_product ( const vec_t &a, const vec_t &b );
	int time_to_ticks ( double time );
	int ticks_to_time ( int ticks );
	void vec_transform ( const float *in1, const matrix3x4_t &in2, float *out );
	void vec_transform ( const vec_t &in1, const matrix3x4_t &in2, vec_t &out );
	float distance_to_ray ( const vec_t &pos, const vec_t &ray_start, const vec_t &ray_end, float *along = 0, vec_t *point_on_ray = 0 );
	float normalize ( float ang );
	vec_t calc_angle ( vec_t src, vec_t dst );
};