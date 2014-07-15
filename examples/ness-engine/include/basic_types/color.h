/**
* Generic color class (4 components of any type to represent a single color)
*
* Author: Ronen Ness
* Since:  26/03/14
*/

#pragma once
#include "../exports.h"
#include <cstdlib>

namespace Ness
{
	/**
	* The Color class is an object that holds 4 color components - red, green, blue and alpha.
	* The type of the components is determine by the template. for example, Color<float> will
	* represent a color made of 4 floats.
	*
	* NOTE: don't use this class directly, use the typedefs at the end of this file instead. for example, "Colorb".
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

		// some useful defaults
		NESSENGINE_API static __Color<type> WHITE;
		NESSENGINE_API static __Color<type> GREY;
		NESSENGINE_API static __Color<type> BLACK;
		NESSENGINE_API static __Color<type> RED;
		NESSENGINE_API static __Color<type> GREEN;
		NESSENGINE_API static __Color<type> BLUE;
		NESSENGINE_API static __Color<type> INVISIBLE;

		// get random color
		static NESSENGINE_API __Color<type> get_random_255() {return __Color<type>((type)(rand() % 255), (type)(rand() % 255), (type)(rand() % 255));}
		static NESSENGINE_API __Color<type> get_random() {return __Color<type>((type)(rand()), (type)(rand()), (type)(rand()));}
	};

	// predefined color types we'll be using
	NESSENGINE_API typedef __Color<int> Colori;
	NESSENGINE_API typedef __Color<unsigned char> Colorb;
	NESSENGINE_API typedef __Color<float> Colorf;
	NESSENGINE_API typedef __Color<float> Color;
};