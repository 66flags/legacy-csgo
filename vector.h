#pragma once

struct vec2_t {
	float x, y;
};

class vec_t {
public:
	vec_t ( ) = default;
	vec_t ( float x, float y, float z ) : x ( x ), y ( y ), z ( z ) { }

	void init ( ) {
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
	}

	bool is_zero ( ) {
		return x == 0.0f && y == 0.0f && z == 0.0f;
	}

	void init ( float x, float y, float z ) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	vec_t ( const float *v ) {
		this->x = v [ 0 ]; this->y = v [ 1 ]; this->z = v [ 2 ];
	}

	float &operator[]( int i ) {
		return ( ( float * ) this ) [ i ];
	}

	float operator[]( int i ) const {
		return ( ( float * ) this ) [ i ];
	}

	void zero ( ) {
		this->x = this->y = this->z = 0.0f;
	}

	bool operator==( const vec_t &src ) const {
		return ( src.x == this->x ) && ( src.y == y ) && ( src.z == z );
	}

	bool operator!=( const vec_t &src ) const {
		return ( src.x != this->x ) || ( src.y != y ) || ( src.z != z );
	}

	vec_t &operator+=( const vec_t &v ) {
		this->x += v.x; this->y += v.y; this->z += v.z;

		return *this;
	}

	vec_t &operator-=( const vec_t &v ) {
		this->x -= v.x; this->y -= v.y; this->z -= v.z;

		return *this;
	}

	vec_t &operator*=( float fl ) {
		this->x *= fl;
		this->y *= fl;
		this->z *= fl;

		return *this;
	}

	vec_t &operator*=( const vec_t &v ) {
		this->x *= v.x;
		this->y *= v.y;
		this->z *= v.z;

		return *this;
	}

	vec_t &operator/=( const vec_t &v ) {
		this->x /= v.x;
		this->y /= v.y;
		this->z /= v.z;

		return *this;
	}

	vec_t &operator+=( float fl ) {
		this->x += fl;
		this->y += fl;
		this->z += fl;

		return *this;
	}

	vec_t &operator/=( float fl ) {
		this->x /= fl;
		this->y /= fl;
		this->z /= fl;

		return *this;
	}

	vec_t &operator-=( float fl ) {
		this->x -= fl;
		this->y -= fl;
		this->z -= fl;

		return *this;
	}

	float length_2d ( ) const {
		return std::sqrtf ( this->x * this->x + this->y * this->y );
	}

	float length ( ) const {
		return std::sqrtf ( this->x * this->x + this->y * this->y + this->z * this->z );
	}

	float dist_to ( const vec_t &other ) {
		vec_t delta { };

		delta.x = this->x - other.x;
		delta.y = this->y - other.y;
		delta.z = this->z - other.z;

		return delta.length ( );
	}

	void normalize ( ) {
		float length = this->length ( );

		if ( length != 0.0f ) {
			this->x /= length + std::numeric_limits< float >::epsilon ( );
			this->y /= length + std::numeric_limits< float >::epsilon ( );
			this->z /= length + std::numeric_limits< float >::epsilon ( );
		}
	}

	float normalize_place ( ) {
		auto radius = std::sqrtf ( x * x + y * y + z * z );
		auto iradius = 1.0f / ( radius + std::numeric_limits< float >::epsilon ( ) );

		x *= iradius;
		y *= iradius;
		z *= iradius;

		return radius;
	}

	vec_t normalized ( ) const {
		auto res = *this;
		auto l = res.length ( );

		if ( l != 0.0f )
			res /= l;
		else
			res.x = res.y = res.z = 0.0f;

		return res;
	}

	vec_t cross ( const vec_t &other ) const {
		return { this->y * other.z - this->z * other.y, this->z * other.x - this->x * other.z, this->x * other.y - this->y * other.x };
	}

	float dot ( const vec_t &vec ) const {
		return { this->x * vec.x + this->y * vec.y + this->z * vec.z };
	}

	float length_2d_sqr ( ) const {
		return this->x * this->x + this->y * this->y;
	}

	float length_sqr ( ) const {
		return this->x * this->x + this->y * this->y + this->z * this->z;
	}

	float x, y, z;

	vec_t &operator=( const vec_t &vec ) {
		this->x = vec.x; this->y = vec.y; this->z = vec.z;

		return *this;
	}

	vec_t operator-( ) const {
		return vec_t ( -this->x, -this->y, -this->z );
	}

	vec_t operator+( const vec_t &v ) const {
		return vec_t ( this->x + v.x, this->y + v.y, this->z + v.z );
	}

	vec_t operator-( const vec_t &v ) const {
		return vec_t ( this->x - v.x, this->y - v.y, this->z - v.z );
	}

	vec_t operator*( float fl ) const {
		return vec_t ( this->x * fl, this->y * fl, this->z * fl );
	}

	vec_t operator*( const vec_t &v ) const {
		return vec_t ( this->x * v.x, this->y * v.y, this->z * v.z );
	}

	vec_t operator/( float fl ) const {
		return vec_t ( this->x / fl, this->y / fl, this->z / fl );
	}

	vec_t operator/( const vec_t &v ) const {
		return vec_t ( this->x / v.x, this->y / v.y, this->z / v.z );
	}
};

__forceinline vec_t operator*( float lhs, const vec_t &rhs ) {
	return rhs * lhs;
}

__forceinline vec_t operator/( float lhs, const vec_t &rhs ) {
	return rhs / lhs;
}

class __declspec( align( 16 ) ) vec_aligned_t : public vec_t {
public:
	inline vec_aligned_t ( ) { }
	inline vec_aligned_t ( float x, float y, float z ) {
		init ( x, y, z );
	}

	explicit vec_aligned_t ( const vec_t &other ) {
		init ( other.x, other.y, other.z );
	}

	vec_aligned_t &operator=( const vec_t &other ) {
		init ( other.x, other.y, other.z );
		return *this;
	}

	float w;
};