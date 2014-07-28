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
	class Rectangle : public SDL_Rect
	{
	public:
		Rectangle()
		{
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
