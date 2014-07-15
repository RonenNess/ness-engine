#include "point.h"
#include "size.h"
#include "rectangle.h"
#include "color.h"

#pragma warning(disable: 4244)
namespace Ness
{

	// Point consts
	#define VECTOR2_CONSTS(__VType)  \
	__VType __VType::ONE(1, 1);	\
	__VType __VType::ZERO(0, 0); \
	__VType __VType::HALF(0.5f, 0.5f)
	VECTOR2_CONSTS(Pointf);
	VECTOR2_CONSTS(Pointi);
	VECTOR2_CONSTS(Pointb);

	// All Point type definitions (to make the dll generate them no matter what)
	template class __Point<int>;
	template class __Point<float>;
	template class __Point<unsigned char>;

	// all the static consts for all color types
	#define COLOR_CONSTS(__CType, __Factor)	\
	__CType __CType::WHITE		((1.0f * __Factor),	(1.0f * __Factor),	(1.0f * __Factor),	(1.0f * __Factor));	\
	__CType __CType::GREY		((0.5f * __Factor),	(0.5f * __Factor),	(0.5f * __Factor),	(1.0f * __Factor));	\
	__CType __CType::BLACK		(0,		0,		0,		(1.0f * __Factor));	\
	__CType __CType::RED		((1.0f * __Factor),	0,		0,		(1.0f * __Factor));	\
	__CType __CType::GREEN		(0,		(1.0f * __Factor),	0,		(1.0f * __Factor));	\
	__CType __CType::BLUE		(0,		0,		(1.0f * __Factor),	(1.0f * __Factor));	\
	__CType __CType::INVISIBLE	(0,		0,		0,		0)

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