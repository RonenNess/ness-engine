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
#include <stdlib.h>

namespace Ness
{
	#define PI 3.14159265f
	#define DEGREE_TO_RADIAN(__degree) (__degree * (PI/180.0f))
	#define RADIAN_TO_DEGREE(__radian) (__radian * (180.0f/PI))
	#define POW2(__val) ((__val)*(__val))

	class Math
	{
	public:

		// convert degree to radian
		NESSENGINE_API inline static float degree_to_radian(int degree)
		{
			return (float)DEGREE_TO_RADIAN(degree);
		}

		// convert degree to radian
		NESSENGINE_API inline static int radian_to_degree(float radian)
		{
			return (int)RADIAN_TO_DEGREE(radian);
		}

		// constrain angle to be between 0 to 360
		NESSENGINE_API inline static int constrain_angle_0_360(int x)
		{
			x = (int)fmod((float)x, (float)360);
			if (x < 0) x += 360;
			return x;
		}

		// constrain angle to be between -180 to 180
		NESSENGINE_API inline static int constrain_angle_180(int x)
		{
			x = (int)fmod((float)x + 180, (float)360);
			if (x < 0) x += 360;
			return x - 180;
		}

		// return a random float number between min and max
		NESSENGINE_API inline static float rand_float(float min = 0.0f, float max = 1.0f)
		{
			return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));
		}
		
		// return a random int number between min and max
		NESSENGINE_API inline static int rand_int(unsigned int min = 0, unsigned int max = 100)
		{
			return min + (rand() % (max - min));
		}
	};
};