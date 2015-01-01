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
#include "string.h"
#include "math.h"
#include <cmath>

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
		NESSENGINE_API inline __Point<type>& add_x(type val) {x += val; return (*this);}
		NESSENGINE_API inline __Point<type>& add_y(type val) {y += val; return (*this);}

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
		NESSENGINE_API inline __Point<type>& limit(type min, type max)
		{
			limitx(min, max);
			limity(min, max);
			return (*this);
		}
		NESSENGINE_API inline __Point<type>& limit(const __Point<type>& min, const __Point<type>& max)
		{
			limitx(min.x, max.x);
			limity(min.y, max.y);
			return (*this);
		}

		// put x value within given limit
		NESSENGINE_API inline __Point<type>& limitx(type min, type max)
		{
			if (x < min) x = min;
			if (x > max) x = max;
			return (*this);
		}

		// put y value within given limit
		NESSENGINE_API inline __Point<type>& limity(type min, type max)
		{
			if (y < min) y = min;
			if (y > max) y = max;
			return (*this);
		}

		// copy/duplicate this point
		NESSENGINE_API inline __Point<type> copy() const
		{
			return *this;
		}

		// get point copy within limit
		NESSENGINE_API inline __Point<type> get_limit(type min, type max) const
		{
			__Point<type> ret = *this;
			ret.limit(min, max);
			return ret;
		}
		NESSENGINE_API inline __Point<type> get_limit(const __Point<type>& min, const __Point<type>& max) const
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
			type div = std::abs(x) + std::abs(y);
			if (div == 0) return __Point<type>::ZERO;
			return __Point<type>(x / div, y / div);
		}

		// return absolute values
		NESSENGINE_API inline __Point<type> get_abs() const 
		{
			return __Point<type>(std::abs(x), std::abs(y));
		}

		// turn to absolute value
		NESSENGINE_API inline __Point<type>& abs() 
		{
			x = std::abs(x);
			y = std::abs(y);
			return (*this);
		}

		// normalize this point
		NESSENGINE_API inline __Point<type>& normalize()
		{
			type div = std::abs(x) + std::abs(y);
			if (div == 0) return (*this);
			x /= div; 
			y /= div;
			return (*this);
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
		NESSENGINE_API inline __Point<type>& rotate(float angle)
		{
			(*this) = __Point<type>::from_angle(this->get_angle() + angle, this->get_length());
			return (*this);
		}

		// ctor
		NESSENGINE_API inline __Point<type>(type X, type Y) : x(X), y(Y)
		{}
		NESSENGINE_API inline __Point<type>() : x(0), y(0)
		{}

		// floor the values of this point
		NESSENGINE_API inline __Point<type>& floor()
		{
			this->x = (type)std::floor((float)this->x);
			this->y = (type)std::floor((float)this->y);
			return (*this);
		}
		NESSENGINE_API inline __Point<type> get_floor() const
		{
			return __Point<type>((*this)).floor();
		}

		// ceil the values of this point
		NESSENGINE_API inline __Point<type>& ceil()
		{
			this->x = (type)std::ceil((float)this->x);
			this->y = (type)std::ceil((float)this->y);
			return (*this);
		}
		NESSENGINE_API inline __Point<type> get_ceil() const
		{
			return __Point<type>((*this)).ceil();
		}

		// limit the number of digits after the decimal point
		// for example, if the point is (x=2.5232, y=0.2511244) and you call .get_round_by(2),
		// the return value will be this point: (x=2.52, y=0.25).
		// Note: obviously useable only for types with floating point (float, double...)
		NESSENGINE_API inline __Point<type> get_round_by(int num_of_zeroes_after_dot = 2) const
		{
			int factor = (int)pow((double)10.0, num_of_zeroes_after_dot);
			return ((__Point<type>((*this) * (type)factor).floor()) / (type)factor);
		}
		NESSENGINE_API inline __Point<type>& round_by(int num_of_zeroes_after_dot = 2)
		{
			*this = this->get_round_by(num_of_zeroes_after_dot);
			return (*this);
		}

		// const useful points
		NESSENGINE_API static __Point<type> ZERO;
		NESSENGINE_API static __Point<type> ONE;
		NESSENGINE_API static __Point<type> HALF;

		// serialize the point into representable string
		NESSENGINE_API String serialize() const
		{
			return ness_float_to_string(x) + "," + ness_float_to_string(y);
		}

		// deserialize the point from a representable string
		NESSENGINE_API void deserialize(const String& src)
		{
			int sep = src.find(',');
			if (sep == -1) throw WrongFormatError("Input string is not a valid point format! serialized point must contain a comma");
			std::string sx = src.substr(0, sep);
			std::string sy = src.substr(sep + 1);
			x = (type)(atof(sx.c_str()));
			y = (type)(atof(sy.c_str()));
		}

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