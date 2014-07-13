/**
* Define the Point class.
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../exports.h"
#include "math.h"

namespace Ness
{
	/**
	* represent a 2d point (x, y)
	*
	* NOTE: don't use this class directly, use the typedefs at the end of this file instead. for example, "Point" or "Pointf".
	*/
	template <typename type>
	class __Point
	{
	public:
		type x;
		type y;

	public:
		NESSENGINE_API inline type get_x() const {return x;}
		NESSENGINE_API inline type get_y() const {return y;}

		NESSENGINE_API inline void add_x(type val) {x += val;}
		NESSENGINE_API inline void add_y(type val) {y += val;}

		// operators with another point
		NESSENGINE_API __Point<type>& operator+=(const __Point<type>& other) {x += other.x; y += other.y; return *this;}
		NESSENGINE_API __Point<type>& operator-=(const __Point<type>& other) {x -= other.x; y -= other.y; return *this;}
		NESSENGINE_API __Point<type>& operator*=(const __Point<type>& other) {x *= other.x; y *= other.y; return *this;}
		NESSENGINE_API __Point<type>& operator/=(const __Point<type>& other) {x /= other.x; y /= other.y; return *this;}
		NESSENGINE_API __Point<type> operator*(const __Point<type>& other) const {return __Point<type>(x * other.x, y * other.y);}
		NESSENGINE_API __Point<type> operator/(const __Point<type>& other) const {return __Point<type>(x / other.x, y / other.y);}
		NESSENGINE_API __Point<type> operator+(const __Point<type>& other) const {return __Point<type>(x + other.x, y + other.y);}
		NESSENGINE_API __Point<type> operator-(const __Point<type>& other) const {return __Point<type>(x - other.x, y - other.y);}

		// operators with scalars
		NESSENGINE_API __Point<type> operator-() const {return __Point<type>(-x, -y);}
		NESSENGINE_API __Point<type>& operator+=(const type& scalar) {x += scalar; y += scalar; return *this;}
		NESSENGINE_API __Point<type>& operator-=(const type& scalar) {x -= scalar; y -= scalar; return *this;}
		NESSENGINE_API __Point<type>& operator*=(const type& scalar) {x *= scalar; y *= scalar; return *this;}
		NESSENGINE_API __Point<type>& operator/=(const type& scalar) {x /= scalar; y /= scalar; return *this;}
		NESSENGINE_API __Point<type> operator*(const type& scalar) const {return __Point<type>(x * scalar, y * scalar);}
		NESSENGINE_API __Point<type> operator/(const type& scalar) const {return __Point<type>(x / scalar, y / scalar);}
		NESSENGINE_API __Point<type> operator+(const type& scalar) const {return __Point<type>(x + scalar, y + scalar);}
		NESSENGINE_API __Point<type> operator-(const type& scalar) const {return __Point<type>(x - scalar, y - scalar);}

		// casting
		NESSENGINE_API operator __Point<float>()	 {return __Point<float>((float)x, (float)y);}
		NESSENGINE_API operator __Point<int>()		 {return __Point<int>((int)x, (int)y);}

		// setter and equel operators
		NESSENGINE_API void set(type X, type Y) { x = X; y = Y;}
		NESSENGINE_API bool operator==(const __Point<type>& other) const {return (x == other.x && y == other.y);}
		NESSENGINE_API bool operator!=(const __Point<type> &other) const {return !(*this == other);}

		// put point within limit
		NESSENGINE_API void limit(type min, type max)
		{
			if (x < min) x = min;
			if (x > max) x = max;
			if (y < min) y = min;
			if (y > max) y = max;
		}

		// ctors
		NESSENGINE_API __Point<type>(type X, type Y) : x(X), y(Y)
		{}
		NESSENGINE_API __Point<type>()
		{}

		// const useful points
		NESSENGINE_API static __Point<type> ZERO;
		NESSENGINE_API static __Point<type> ONE;
		NESSENGINE_API static __Point<type> HALF;

		// static function: create point from angle
		NESSENGINE_API static __Point<type> from_angle(int angle, float velocity = 1.0f) 
		{
			__Point<type> ret;
			ret.x = (type)(cos(DEGREE_TO_RADIAN(angle)) * velocity);
			ret.y = (type)(sin(DEGREE_TO_RADIAN(angle)) * velocity);
			return ret;
		}
	};

	NESSENGINE_API typedef __Point<float> Pointf;
	NESSENGINE_API typedef __Point<float> Point;
	NESSENGINE_API typedef __Point<int> Pointi;
	NESSENGINE_API typedef __Point<unsigned char> Pointb;

};