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
	template <typename Type>
	class __Color
	{
	public:

		// the components of the color
		Type r;
		Type g;
		Type b;
		Type a;

		// += and -= operators
		NESSENGINE_API __Color<Type>& operator+=(const __Color<Type>& other) {r += other.r; g += other.g; b += other.b; a += other.a; return *this;}
		NESSENGINE_API __Color<Type>& operator-=(const __Color<Type>& other) {r -= other.r; g -= other.g; b -= other.b; a -= other.a; return *this;}
		NESSENGINE_API __Color<Type>& operator*=(const __Color<Type>& other) {r *= other.r; g *= other.g; b *= other.b; a *= other.a; return *this;}
		NESSENGINE_API __Color<Type>& operator/=(const __Color<Type>& other) {r /= other.r; g /= other.g; b /= other.b; a /= other.a; return *this;}

		// create empty color (with garbage values)
		NESSENGINE_API __Color<Type>()
		{
		}

		// create color with values
		NESSENGINE_API __Color<Type>(Type R, Type G, Type B, Type A = 1.0) : r(R), g(G), b(B), a(A)
		{}

		// get base type
		NESSENGINE_API const type_info& get_component_type() const {return typeid(Type);}

		// set all color components
		NESSENGINE_API inline void set(Type R, Type G, Type B, Type A = 1.0) {r = R; g = G; b = B; a = A;}

		// some useful defaults
		NESSENGINE_API static __Color<Type> WHITE;
		NESSENGINE_API static __Color<Type> GREY;
		NESSENGINE_API static __Color<Type> BLACK;
		NESSENGINE_API static __Color<Type> RED;
		NESSENGINE_API static __Color<Type> GREEN;
		NESSENGINE_API static __Color<Type> BLUE;
		NESSENGINE_API static __Color<Type> INVISIBLE;

		// get random color
		static NESSENGINE_API __Color<Type> get_random_255() {return __Color<Type>((Type)(rand() % 255), (Type)(rand() % 255), (Type)(rand() % 255));}
		static NESSENGINE_API __Color<Type> get_random() {return __Color<Type>((Type)(rand()), (Type)(rand()), (Type)(rand()));}
	};

	// predefined color types we'll be using
	NESSENGINE_API typedef __Color<int> Colori;
	NESSENGINE_API typedef __Color<unsigned char> Colorb;
	NESSENGINE_API typedef __Color<float> Colorf;
	NESSENGINE_API typedef __Color<float> Color;
};