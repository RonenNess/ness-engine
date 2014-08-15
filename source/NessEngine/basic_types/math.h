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

#pragma once
#include <math.h>

namespace Ness
{
	#define PI 3.14159265
	#define DEGREE_TO_RADIAN(__degree) (__degree * (PI/180))
	#define RADIAN_TO_DEGREE(__radian) (__radian * (180 / PI))
	#define POW2(__val) ((__val)*(__val))
};