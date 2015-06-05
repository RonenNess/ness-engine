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

#include "point.h"
#include "size.h"
#include "rectangle.h"
#include "color.h"

#ifdef _MSC_VER
#define TEMPLATE
#else
#define TEMPLATE template<>
#endif

#pragma warning(disable: 4244)
namespace Ness
{

	// Point consts
#define VECTOR2_CONSTS(__VType)      \
        TEMPLATE \
         __VType __VType::ONE(1, 1);	\
        TEMPLATE \
         __VType __VType::ZERO(0, 0); \
        TEMPLATE \
         __VType __VType::HALF(0.5f, 0.5f);

	VECTOR2_CONSTS(Pointf);
	VECTOR2_CONSTS(Pointi);
	VECTOR2_CONSTS(Pointb);

	// All Point type definitions (to make the dll generate them no matter what)
	template class __Point<int>;
	template class __Point<float>;
	template class __Point<unsigned char>;

	// all the static consts for all color types
#define COLOR_CONSTS(__CType, __Factor)	\
        TEMPLATE \
	 __CType __CType::WHITE((1.0f * __Factor), (1.0f * __Factor), (1.0f * __Factor), (1.0f * __Factor));	\
        TEMPLATE \
	 __CType __CType::GREY((0.5f * __Factor), (0.5f * __Factor), (0.5f * __Factor), (1.0f * __Factor));	\
        TEMPLATE \
	 __CType __CType::BLACK(0, 0, 0, (1.0f * __Factor));	\
        TEMPLATE \
	 __CType __CType::RED((1.0f * __Factor), 0, 0, (1.0f * __Factor));	\
        TEMPLATE \
	 __CType __CType::GREEN(0, (1.0f * __Factor), 0, (1.0f * __Factor));	\
        TEMPLATE \
	 __CType __CType::BLUE(0, 0, (1.0f * __Factor), (1.0f * __Factor));	\
        TEMPLATE \
	 __CType __CType::INVISIBLE(0, 0, 0, 0);	\
        TEMPLATE \
	 __CType __CType::HALF_INVISIBLE(0, 0, 0, (0.5f * __Factor)); \
        TEMPLATE \
	 __CType __CType::YELLOW((1.0f * __Factor), (1.0f * __Factor), 0, (1.0f * __Factor)); \
        TEMPLATE \
	 __CType __CType::CYAN(0.0f, (1.0f * __Factor), (1.0f * __Factor), (1.0f * __Factor)); \
        TEMPLATE \
	 __CType __CType::MAGENTA((1.0f * __Factor), 0.0f, (1.0f * __Factor), (1.0f * __Factor));


	COLOR_CONSTS(Colorf, 1.0f);
	COLOR_CONSTS(Colorb, 255);
	COLOR_CONSTS(Colori, 255);

	// since templates are instanciated only at compile time, the following statements force the compiler to generate the code for these specific types.
	// without these we'll get unresolved externals when trying to use these colors:
	template class __Color<float>;
	template class __Color<unsigned char>;
	template class __Color<int>;
};
#pragma warning(default: 4244)
