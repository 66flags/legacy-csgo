#include "includes.h"

int math::time_to_ticks ( double time ) {
	return static_cast < int > ( 0.5f + static_cast< float > ( time ) / interfaces::m_globals->m_interval_per_tick );
}

int math::ticks_to_time ( int ticks ) {
	return static_cast < int > ( ticks * interfaces::m_globals->m_interval_per_tick );
}

void math::vec_transform ( const float *in1, const matrix3x4_t &in2, float *out ) {
	out [ 0 ] = dot_product ( in1, in2 [ 0 ] ) + in2 [ 0 ][ 3 ];
	out [ 1 ] = dot_product ( in1, in2 [ 1 ] ) + in2 [ 1 ][ 3 ];
	out [ 2 ] = dot_product ( in1, in2 [ 2 ] ) + in2 [ 2 ][ 3 ];
}

void math::vec_transform ( const vec_t &in1, const matrix3x4_t &in2, vec_t &out ) {
	vec_transform ( &in1.x, in2, &out.x );
}

float math::distance_to_ray ( const vec_t &pos, const vec_t &ray_start, const vec_t &ray_end, float *along, vec_t *point_on_ray ) {
	const vec_t to = ( pos - ray_start );
	vec_t dir = ( ray_end - ray_start );

	float length = dir.normalize_place ( );
	float range_along = dir.dot ( to );

	if ( along )
		*along = range_along;

	float range;

	if ( range_along < 0.0f )
		range = -( to ).length ( );
	else if ( range_along > length )
		range = -( pos - ray_end ).length ( );
	else
		range = ( pos - ( dir * range_along + ray_start ) ).length ( );

	return range;
}

vec_t math::angle_vector ( vec_t angle ) {
	vec_t ret;

	float sp, sy, cp, cy;

	sin_cos ( deg2rad ( angle.y ), &sy, &cy );
	sin_cos ( deg2rad ( angle.x ), &sp, &cp );

	ret.x = cp * cy;
	ret.y = cp * sy;
	ret.z = -sp;

	return ret;
}

void math::angle_vectors ( const vec_t &angle, vec_t *forward, vec_t *right, vec_t *up ) {
	float sp, sy, sr, cp, cy, cr;

	sin_cos ( deg2rad ( angle.x ), &sp, &cp );
	sin_cos ( deg2rad ( angle.y ), &sy, &cy );
	sin_cos ( deg2rad ( angle.z ), &sr, &cr );

	if ( forward != nullptr ) {
		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	if ( right != nullptr ) {
		right->x = -1 * sr * sp * cy + -1 * cr * -sy;
		right->y = -1 * sr * sp * sy + -1 * cr * cy;
		right->z = -1 * sr * cp;
	}

	if ( up != nullptr ) {
		up->x = cr * sp * cy + -sr * -sy;
		up->y = cr * sp * sy + -sr * cy;
		up->z = cr * cp;
	}
}

void math::normalize ( vec_t &ang ) {
	for ( auto i = 0; i < 3; i++ ) {
		while ( ang [ i ] < -180.0f ) ang [ i ] += 360.0f;
		while ( ang [ i ] > 180.0f ) ang [ i ] -= 360.0f;
	}
}

void math::sin_cos ( float radians, float *sin, float *cos ) {
	*sin = std::sin ( radians );
	*cos = std::cos ( radians );
}

float math::dot_product ( const vec_t &a, const vec_t &b ) {
	return { a.x * b.x + a.y * b.y + a.z * b.z };
}

vec_t math::vector_angle ( vec_t vec ) {
	constexpr float pi = 3.14159265358979323846;
	
	return vec_t (
		-atan2 ( vec.z, vec.length_2d ( ) ) * 180 / pi,
		atan2 ( vec.y, vec.x ) * 180 / pi,
		0
	);
}

void math::clamp ( vec_t &ang ) {
	auto flt_valid = [ ] ( float val ) {
		return std::isfinite ( val ) && !std::isnan ( val );
	};

	for ( auto i = 0; i < 3; i++ )
		if ( !flt_valid ( ang [ i ] ) )
			ang [ i ] = 0.0f;

	ang.x = std::clamp ( normalize ( ang.x ), -89.0f, 89.0f );
	ang.y = std::clamp ( normalize ( ang.y ), -180.0f, 180.0f );
	ang.z = 0.0f;
}

float math::normalize ( float ang ) {
	if ( isnan ( ang ) || isinf ( ang ) )
		ang = 0.0f;

	while ( ang > 180.0f )
		ang -= 360.0f;

	while ( ang < -180.0f )
		ang += 360.0f;

	return ang;
}

vec_t math::calc_angle ( vec_t src, vec_t dst ) {
	auto ret = vector_angle ( dst - src );

	clamp ( ret );

	return ret;
}