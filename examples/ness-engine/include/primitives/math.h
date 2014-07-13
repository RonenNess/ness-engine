#pragma once
#include <math.h>

namespace Ness
{
	#define PI 3.14159265
	#define DEGREE_TO_RADIAN(__degree) (__degree * (PI/180))
	#define RADIAN_TO_DEGREE(__radian) (__radian * (180 / PI))
	#define POW2(__val) ((__val)*(__val))
};