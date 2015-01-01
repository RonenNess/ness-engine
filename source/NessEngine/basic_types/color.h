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
* Generic color class (4 components of any type to represent a single color)
*
* Author: Ronen Ness
* Since:  26/03/14
*/

#pragma once
#include "../exports.h"
#include "string.h"
#include <cstdlib>

namespace Ness
{
	/**
	* The __Color class is a template that holds 4 color components - red, green, blue and alpha.
	* NOTE: don't use this class directly, use the typedefs at the end of this file instead. for example, "Color" or "Colorb".
	*/
	template <typename type>
	class __Color
	{
	public:

		// the components of the color
		type r;
		type g;
		type b;
		type a;

		// operators with other colors
		NESSENGINE_API __Color<type>& operator+=(const __Color<type>& other) {r += other.r; g += other.g; b += other.b; a += other.a; return *this;}
		NESSENGINE_API __Color<type>& operator-=(const __Color<type>& other) {r -= other.r; g -= other.g; b -= other.b; a -= other.a; return *this;}
		NESSENGINE_API __Color<type>& operator*=(const __Color<type>& other) {r *= other.r; g *= other.g; b *= other.b; a *= other.a; return *this;}
		NESSENGINE_API __Color<type>& operator/=(const __Color<type>& other) {r /= other.r; g /= other.g; b /= other.b; a /= other.a; return *this;}
		NESSENGINE_API __Color<type> operator*(const __Color<type>& other) const {return __Color<type>(r * other.r, g * other.g, b * other.b, a * other.a);}
		NESSENGINE_API __Color<type> operator/(const __Color<type>& other) const {return __Color<type>(r / other.r, g / other.g, b / other.b, a / other.a);}
		NESSENGINE_API __Color<type> operator+(const __Color<type>& other) const {return __Color<type>(r + other.r, g + other.g, b + other.b, a + other.a);}
		NESSENGINE_API __Color<type> operator-(const __Color<type>& other) const {return __Color<type>(r - other.r, g - other.g, b - other.b, a - other.a);}

		// operators with scalars
		NESSENGINE_API __Color<type>& operator+=(const type& scalar) {r += scalar; g += scalar; b += scalar; a += scalar; return *this;}
		NESSENGINE_API __Color<type>& operator-=(const type& scalar) {r -= scalar; g -= scalar; b -= scalar; a -= scalar; return *this;}
		NESSENGINE_API __Color<type>& operator*=(const type& scalar) {r *= scalar; g *= scalar; b *= scalar; a *= scalar; return *this;}
		NESSENGINE_API __Color<type>& operator/=(const type& scalar) {r /= scalar; g /= scalar; b /= scalar; a /= scalar; return *this;}
		NESSENGINE_API __Color<type> operator*(const type& scalar) const {return __Color<type>(r * scalar, g * scalar, b * scalar, a * scalar);}
		NESSENGINE_API __Color<type> operator/(const type& scalar) const {return __Color<type>(r / scalar, g / scalar, b / scalar, a / scalar);}
		NESSENGINE_API __Color<type> operator+(const type& scalar) const {return __Color<type>(r + scalar, g + scalar, b + scalar, a + scalar);}
		NESSENGINE_API __Color<type> operator-(const type& scalar) const {return __Color<type>(r - scalar, g - scalar, b - scalar, a - scalar);}

		// equal operator
		NESSENGINE_API bool operator==(const __Color<type>& other) const {return (memcmp(this, &other, sizeof(__Color<type>)) == 0);}

		// create empty color (with garbage values)
		NESSENGINE_API __Color<type>()
		{
		}

		// create color with values
		NESSENGINE_API __Color<type>(type R, type G, type B, type A = 1.0) : r(R), g(G), b(B), a(A)
		{}

		// get base type
		NESSENGINE_API const type_info& get_component_type() const {return typeid(type);}

		// set all color components
		NESSENGINE_API inline void set(type R, type G, type B, type A = 1.0) {r = R; g = G; b = B; a = A;}

		// put color within limit
		NESSENGINE_API void limit(type min, type max)
		{
			if (r < min) r = min;
			if (r > max) r = max;
			if (g < min) g = min;
			if (g > max) g = max;
			if (b < min) b = min;
			if (b > max) b = max;
			if (a < min) a = min;
			if (a > max) a = max;
		}

		// get point copy within limit
		NESSENGINE_API inline __Color<type> get_limit(type min, type max) const
		{
			__Color<type> ret = *this;
			ret.limit(min, max);
			return ret;
		}

		// some useful defaults
		NESSENGINE_API static __Color<type> WHITE;
		NESSENGINE_API static __Color<type> GREY;
		NESSENGINE_API static __Color<type> BLACK;
		NESSENGINE_API static __Color<type> RED;
		NESSENGINE_API static __Color<type> GREEN;
		NESSENGINE_API static __Color<type> BLUE;
		NESSENGINE_API static __Color<type> YELLOW;
		NESSENGINE_API static __Color<type> CYAN;
		NESSENGINE_API static __Color<type> MAGENTA;
		NESSENGINE_API static __Color<type> INVISIBLE;
		NESSENGINE_API static __Color<type> HALF_INVISIBLE;

		// serialize the color and return a string representation of it
		// note: only works for float color
		NESSENGINE_API String serialize() const
		{
			String ret = "#";
			String temp;
			char arr[3];
			_itoa_s ((int)(r * 255.0f), arr, 16);
			temp = arr;
			if (temp.length() == 1) temp = '0' + temp;
			ret.append(temp);
			_itoa_s ((int)(g * 255.0f), arr, 16);
			temp = arr;
			if (temp.length() == 1) temp = '0' + temp;
			ret.append(temp);
			_itoa_s ((int)(b * 255.0f), arr, 16);
			temp = arr;
			if (temp.length() == 1) temp = '0' + temp;
			ret.append(temp);
			_itoa_s ((int)(a * 255.0f), arr, 16);
			temp = arr;
			if (temp.length() == 1) temp = '0' + temp;
			ret.append(temp);
			return ret;
		}

		// deserialize the color from a string representation
		// note: only works for float color
		NESSENGINE_API void deserialize(const String& src)
		{
			// take the first 9 chars out of the src string: '#rrggbbaa' = 9 chars
			String tmp = src.substr(0, 9);

			// make sure string starts with '#' and remove it
			if (tmp.at(0) != '#')
			{
				throw IllegalAction("Input string is not a valid color format! serialized color must begin with #.");
			}
			// make sure have enough digits
			if (tmp.length() != 9)
			{
				throw IllegalAction("Input string is not a valid color format! serialized color must be 9 digits (#rrggbbaa)");
			}

			std::string rs = value.substr(1, 2);
			std::string gs = value.substr(3, 2);
			std::string bs = value.substr(5, 2);
			std::string as = value.substr(7, 2);
			r = (float)std::stoul(rs.c_str(), nullptr, 16) / 255.0f;
			g = (float)std::stoul(gs.c_str(), nullptr, 16) / 255.0f;
			b = (float)std::stoul(bs.c_str(), nullptr, 16) / 255.0f;
			a = (float)std::stoul(as.c_str(), nullptr, 16) / 255.0f;
		}

		// get random color from 0.0f to 1.0f
		static NESSENGINE_API __Color<float> get_random() {return __Color<float>(Math::rand_float(), Math::rand_float(), Math::rand_float());}

		// get random color from 0 to 255
		static NESSENGINE_API __Color<type> get_random_255() {return __Color<type>((type)(rand() % 256), (type)(rand() % 256), (type)(rand() % 256));}
	};

	// predefined color types we'll be using
	NESSENGINE_API typedef __Color<int> Colori;
	NESSENGINE_API typedef __Color<unsigned char> Colorb;
	NESSENGINE_API typedef __Color<float> Colorf;
	NESSENGINE_API typedef __Color<float> Color;
};