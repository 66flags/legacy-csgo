#pragma once

class matrix3x4a_t;

struct matrix3x4_t {
	matrix3x4_t ( ) { }

	matrix3x4_t (
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23 ) {
		m_matrix [ 0 ][ 0 ] = m00;	m_matrix [ 0 ][ 1 ] = m01; m_matrix [ 0 ][ 2 ] = m02; m_matrix [ 0 ][ 3 ] = m03;
		m_matrix [ 1 ][ 0 ] = m10;	m_matrix [ 1 ][ 1 ] = m11; m_matrix [ 1 ][ 2 ] = m12; m_matrix [ 1 ][ 3 ] = m13;
		m_matrix [ 2 ][ 0 ] = m20;	m_matrix [ 2 ][ 1 ] = m21; m_matrix [ 2 ][ 2 ] = m22; m_matrix [ 2 ][ 3 ] = m23;
	}

	void init_xyz ( const vec_t &x_axis, const vec_t &y_axis, const vec_t &z_axis, const vec_t &origin ) {
		m_matrix [ 0 ][ 0 ] = x_axis.x; m_matrix [ 0 ][ 1 ] = y_axis.x; m_matrix [ 0 ][ 2 ] = z_axis.x; m_matrix [ 0 ][ 3 ] = origin.x;
		m_matrix [ 1 ][ 0 ] = x_axis.y; m_matrix [ 1 ][ 1 ] = y_axis.y; m_matrix [ 1 ][ 2 ] = z_axis.y; m_matrix [ 1 ][ 3 ] = origin.y;
		m_matrix [ 2 ][ 0 ] = x_axis.z; m_matrix [ 2 ][ 1 ] = y_axis.z; m_matrix [ 2 ][ 2 ] = z_axis.z; m_matrix [ 2 ][ 3 ] = origin.z;
	}

	void init ( const vec_t &x_axis, const vec_t &y_axis, const vec_t &z_axis, const vec_t &origin ) {
		m_matrix [ 0 ][ 0 ] = x_axis.x; m_matrix [ 0 ][ 1 ] = y_axis.x; m_matrix [ 0 ][ 2 ] = z_axis.x; m_matrix [ 0 ][ 3 ] = origin.x;
		m_matrix [ 1 ][ 0 ] = x_axis.y; m_matrix [ 1 ][ 1 ] = y_axis.y; m_matrix [ 1 ][ 2 ] = z_axis.y; m_matrix [ 1 ][ 3 ] = origin.y;
		m_matrix [ 2 ][ 0 ] = x_axis.z; m_matrix [ 2 ][ 1 ] = y_axis.z; m_matrix [ 2 ][ 2 ] = z_axis.z; m_matrix [ 2 ][ 3 ] = origin.z;
	}

	matrix3x4_t ( const vec_t &x_axis, const vec_t &y_axis, const vec_t &z_axis, const vec_t &origin ) {
		init ( x_axis, y_axis, z_axis, origin );
	}

	__forceinline void set_origin ( vec_t const &p ) {
		m_matrix [ 0 ][ 3 ] = p.x;
		m_matrix [ 1 ][ 3 ] = p.y;
		m_matrix [ 2 ][ 3 ] = p.z;
	}

	__forceinline vec_t get_origin ( void ) const {
		vec_t ret ( m_matrix [ 0 ][ 3 ], m_matrix [ 1 ][ 3 ], m_matrix [ 2 ][ 3 ] );
		return ret;
	}

	__forceinline void invalidate_matrix ( void ) {
		for ( int i = 0; i < 3; i++ ) {
			for ( int j = 0; j < 4; j++ ) {
				m_matrix [ i ][ j ] = FLT_MAX;
			}
		}
	}

	__forceinline bool is_valid ( void ) const {
		for ( int i = 0; i < 3; i++ ) {
			for ( int j = 0; j < 4; j++ ) {
				if ( !std::isfinite ( m_matrix [ i ][ j ] ) )
					return false;
			}
		}
		return true;
	}

	__forceinline bool operator==( const matrix3x4_t &other ) const {
		return memcmp ( this, &other, sizeof ( matrix3x4_t ) ) == 0;
	}

	__forceinline bool operator!=( const matrix3x4_t &other ) const {
		return memcmp ( this, &other, sizeof ( matrix3x4_t ) ) != 0;
	}

	float *operator[]( int i ) { return m_matrix [ i ]; }
	const float *operator[]( int i ) const { return m_matrix [ i ]; }
	float *base ( ) { return &m_matrix [ 0 ][ 0 ]; }
	const float *base ( ) const { return &m_matrix [ 0 ][ 0 ]; }

	float m_matrix [ 3 ][ 4 ];
};

class __declspec( align( 16 ) ) matrix3x4a_t : public matrix3x4_t {
public:
	matrix3x4a_t ( const matrix3x4_t &src ) { *this = src; };
	matrix3x4a_t &operator=( const matrix3x4_t &src ) { memcpy ( base ( ), src.base ( ), sizeof ( float ) * 3 * 4 ); return *this; };

	matrix3x4a_t (
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23 ) {
		m_matrix [ 0 ][ 0 ] = m00;	m_matrix [ 0 ][ 1 ] = m01; m_matrix [ 0 ][ 2 ] = m02; m_matrix [ 0 ][ 3 ] = m03;
		m_matrix [ 1 ][ 0 ] = m10;	m_matrix [ 1 ][ 1 ] = m11; m_matrix [ 1 ][ 2 ] = m12; m_matrix [ 1 ][ 3 ] = m13;
		m_matrix [ 2 ][ 0 ] = m20;	m_matrix [ 2 ][ 1 ] = m21; m_matrix [ 2 ][ 2 ] = m22; m_matrix [ 2 ][ 3 ] = m23;
	}
	matrix3x4a_t ( ) { }
};

class view_matrix_t {
public:
	__forceinline float *operator[]( int i ) {
		return m [ i ];
	}
	__forceinline const float *operator[]( int i ) const {
		return m [ i ];
	}

	__forceinline float *base ( ) {
		return &m [ 0 ][ 0 ];
	}
	__forceinline const float *base ( ) const {
		return &m [ 0 ][ 0 ];
	}
public:
	float m [ 4 ][ 4 ];
};