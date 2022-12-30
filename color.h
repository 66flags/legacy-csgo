#pragma once

struct color_t {
	color_t ( ) = default;

	template < typename t >
	__forceinline color_t ( t r, t g, t b ) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = 255;
	}

	template < typename t >
	__forceinline color_t ( t r, t g, t b, t a ) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	template < typename t >
	__forceinline color_t ( color_t col, t a ) {
		this->r = col.r;
		this->g = col.g;
		this->b = col.b;
		this->a = a;
	}

	constexpr void swap ( float &a, float &b ) { float tmp = a; a = b; b = tmp; }
	constexpr float colfabs ( const float &x ) { return x < 0 ? x * -1 : x; }

	constexpr color_t &from_hsv ( float h, float s, float v ) {
		float out [ 3 ] { };

		if ( s == 0.0f ) {
			r = g = b = static_cast< float >( v * 255 );
			return *this;
		}

		h = std::fmodf ( h, 1.0f ) / ( 60.0f / 360.0f );

		int i = static_cast< float >( h );
		float f = h - static_cast< float >( i );
		float p = v * ( 1.0f - s );
		float q = v * ( 1.0f - s * f );
		float t = v * ( 1.0f - s * ( 1.0f - f ) );

		switch ( i ) {
		case 0:
			out [ 0 ] = v;
			out [ 1 ] = t;
			out [ 2 ] = p;
			break;
		case 1:
			out [ 0 ] = q;
			out [ 1 ] = v;
			out [ 2 ] = p;
			break;
		case 2:
			out [ 0 ] = p;
			out [ 1 ] = v;
			out [ 2 ] = t;
			break;
		case 3:
			out [ 0 ] = p;
			out [ 1 ] = q;
			out [ 2 ] = v;
			break;
		case 4:
			out [ 0 ] = t;
			out [ 1 ] = p;
			out [ 2 ] = v;
			break;
		case 5: default:
			out [ 0 ] = v;
			out [ 1 ] = p;
			out [ 2 ] = q;
			break;
		}

		r = static_cast< float >( out [ 0 ] * 255 );
		g = static_cast< float >( out [ 1 ] * 255 );
		b = static_cast< float >( out [ 2 ] * 255 );

		return *this;
	}

	constexpr auto to_hsv ( float &h, float &s, float &v ) {
		float col [ 3 ] = { r / 255.f, g / 255.f, b / 255.f };

		float K = 0.f;

		if ( col [ 1 ] < col [ 2 ] ) {
			swap ( col [ 1 ], col [ 2 ] );
			K = -1.f;
		}

		if ( col [ 0 ] < col [ 1 ] ) {
			swap ( col [ 0 ], col [ 1 ] );
			K = -2.f / 6.f - K;
		}

		const float chroma = col [ 0 ] - ( col [ 1 ] < col [ 2 ] ? col [ 1 ] : col [ 2 ] );
		h = colfabs ( K + ( col [ 1 ] - col [ 2 ] ) / ( 6.f * chroma + 1e-20f ) );
		s = chroma / ( col [ 0 ] + 1e-20f );
		v = col [ 0 ];
	}

	__forceinline float red ( ) { return r; }
	__forceinline float green ( ) { return g; }
	__forceinline float blue ( ) { return b; }
	__forceinline float alpha ( ) { return a; }

	unsigned char r, g, b, a;
};