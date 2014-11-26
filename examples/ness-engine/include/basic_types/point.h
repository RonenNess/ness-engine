/* 
  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Ronen Ness
  ronenness@gmail.com

*/

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
	* represent a 2d point or a vector (x, y)
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

		// getters
		NESSENGINE_API inline type get_x() const {return x;}
		NESSENGINE_API inline type get_y() const {return y;}

		// add to x or y
		NESSENGINE_API inline void add_x(type val) {x += val;}
		NESSENGINE_API inline void add_y(type val) {y += val;}

		// operators with another point
		NESSENGINE_API inline __Point<type>& operator+=(const __Point<type>& other) {x += other.x; y += other.y; return *this;}
		NESSENGINE_API inline __Point<type>& operator-=(const __Point<type>& other) {x -= other.x; y -= other.y; return *this;}
		NESSENGINE_API inline __Point<type>& operator*=(const __Point<type>& other) {x *= other.x; y *= other.y; return *this;}
		NESSENGINE_API inline __Point<type>& operator/=(const __Point<type>& other) {x /= other.x; y /= other.y; return *this;}
		NESSENGINE_API inline __Point<type>& operator%=(const __Point<type>& other) {x = (type)((int)x % (int)other.x); y = (type)((int)y % (int)other.y); return *this;}
		NESSENGINE_API inline __Point<type> operator*(const __Point<type>& other) const {return __Point<type>(x * other.x, y * other.y);}
		NESSENGINE_API inline __Point<type> operator/(const __Point<type>& other) const {return __Point<type>(x / other.x, y / other.y);}
		NESSENGINE_API inline __Point<type> operator+(const __Point<type>& other) const {return __Point<type>(x + other.x, y + other.y);}
		NESSENGINE_API inline __Point<type> operator-(const __Point<type>& other) const {return __Point<type>(x - other.x, y - other.y);}
		NESSENGINE_API inline __Point<type> operator%(const __Point<type>& other) const {return __Point<type>((type)((int)x % (int)other.x), (type)((int)y % (int)other.y));}

		// operators with scalars
		NESSENGINE_API inline __Point<type> operator-() const {return __Point<type>(-x, -y);}
		NESSENGINE_API inline __Point<type>& operator+=(const type& scalar) {x += scalar; y += scalar; return *this;}
		NESSENGINE_API inline __Point<type>& operator-=(const type& scalar) {x -= scalar; y -= scalar; return *this;}
		NESSENGINE_API inline __Point<type>& operator*=(const type& scalar) {x *= scalar; y *= scalar; return *this;}
		NESSENGINE_API inline __Point<type>& operator/=(const type& scalar) {x /= scalar; y /= scalar; return *this;}
		NESSENGINE_API inline __Point<type>& operator%=(const type& scalar) {x = (type)((int)x % (int)scalar); y = (type)((int)y % (int)scalar); return *this;}
		NESSENGINE_API inline __Point<type> operator*(const type& scalar) const {return __Point<type>(x * scalar, y * scalar);}
		NESSENGINE_API inline __Point<type> operator/(const type& scalar) const {return __Point<type>(x / scalar, y / scalar);}
		NESSENGINE_API inline __Point<type> operator+(const type& scalar) const {return __Point<type>(x + scalar, y + scalar);}
		NESSENGINE_API inline __Point<type> operator-(const type& scalar) const {return __Point<type>(x - scalar, y - scalar);}
		NESSENGINE_API inline __Point<type> operator%(const type& scalar) const {return __Point<type>((type)((int)x % (int)scalar), (type)((int)y % (int)scalar));}

		// casting
		NESSENGINE_API inline operator __Point<float>() const	 {return __Point<float>((float)x, (float)y);}
		NESSENGINE_API inline operator __Point<int>()	const	 {return __Point<int>((int)x, (int)y);}

		// setter and equel operators
		NESSENGINE_API inline void set(type X, type Y) { x = X; y = Y;}
		NESSENGINE_API inline bool operator==(const __Point<type>& other) const {return (x == other.x && y == other.y);}
		NESSENGINE_API inline bool operator!=(const __Point<type> &other) const {return !(*this == other);}

		// return the angle between this vector and the x axis
		// note: value will range between 180 to -180, while 0 is when vector is (1, 0)
		NESSENGINE_API inline int get_angle() const {return Math::constrain_angle_180((int)RADIAN_TO_DEGREE(get_radian()));}
		NESSENGINE_API inline float get_radian() const {return (float)atan2((float)y, (float)x);}

		// put point within limit
		NESSENGINE_API inline void limit(type min, type max)
		{
			limitx(min, max);
			limity(min, max);
		}

		NESSENGINE_API inline void limitx(type min, type max)
		{
			if (x < min) x = min;
			if (x > max) x = max;
		}

		NESSENGINE_API inline void limity(type min, type max)
		{
			if (y < min) y = min;
			if (y > max) y = max;
		}

		// get point copy within limit
		NESSENGINE_API inline __Point<type> get_limit(type min, type max) const
		{
			__Point<type> ret = *this;
			ret.limit(min, max);
			return ret;
		}

		// return distance from other point
		NESSENGINE_API inline float distance(const __Point<type>& other) const 
		{
			return sqrt((float)(POW2(x - other.x) + POW2(y - other.y)));
		}

		// return normalized point
		NESSENGINE_API inline __Point<type> get_normalized() const 
		{
			type div = abs(x) + abs(y);
			if (div == 0) return __Point<type>::ZERO;
			return __Point<type>(x / div, y / div);
		}

		// return absolute values
		NESSENGINE_API inline __Point<type> get_abs() const 
		{
			return __Point<type>(abs(x), abs(y));
		}

		// turn to absolute value
		NESSENGINE_API inline void make_abs() 
		{
			x = abs(x);
			y = abs(y);
		}

		// normalize this point
		NESSENGINE_API inline void normalize()
		{
			type div = abs(x) + abs(y);
			if (div == 0) return;
			x /= div; 
			y /= div;
		}

		// return the length of this vector
		NESSENGINE_API inline float get_length() const
		{
			return sqrt(pow((float)x, 2) + pow((float)y, 2));
		}

		// return a rotated version of this vector
		NESSENGINE_API inline __Point<type> get_rotated(float angle) const
		{
			return __Point<type>::from_angle(this->get_angle() + angle, this->get_length());
		}

		// rotate this vector
		NESSENGINE_API inline void rotate(float angle)
		{
			(*this) = __Point<type>::from_angle(this->get_angle() + angle, this->get_length());
		}

		// ctor
		NESSENGINE_API inline __Point<type>(type X = 0, type Y = 0) : x(X), y(Y)
		{}

		// const useful points
		NESSENGINE_API static __Point<type> ZERO;
		NESSENGINE_API static __Point<type> ONE;
		NESSENGINE_API static __Point<type> HALF;

		// static function: create point from angle
		NESSENGINE_API static __Point<type> from_angle(float angle, float velocity = 1.0f) 
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