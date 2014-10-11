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
* Define the rectangle class.
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <SDL.h>
#include "point.h"
#include "size.h"
#include "../exports.h"

namespace Ness
{
	// represent a position + size rectangle
	// extends the SDL_Rect struct
	class Rectangle : public SDL_Rect
	{
	public:
		Rectangle()
		{
			x = 0; y = 0; w = 0; h = 0;
		}

		Rectangle(int X, int Y, int W, int H)
		{ 
			x = X; y = Y; w = W; h = H;
		}

		Rectangle(const Pointi& pos, const Sizei& size)
		{ 
			x = pos.x; y = pos.y; w = size.x; h = size.y;
		}
	};
};
