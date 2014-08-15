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
* renderable basic shapes
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "entity.h"

namespace Ness
{

	// the renderable rectangle with color, either filled or just lines
	class RectangleShape: public Entity
	{
	private:
		bool m_is_filled;

	public:
		// create the rectangle shape
		NESSENGINE_API RectangleShape(Renderer* renderer);

		// set if this rectangle is filled or just outlines
		NESSENGINE_API inline void set_filled(bool filled) {m_is_filled = filled;}

	protected:
		// the actual rendering function
		NESSENGINE_API virtual void do_render(const Rectangle& target, const SRenderTransformations& transformations);
	};

	// rectangle shape pointer
	NESSENGINE_API typedef SharedPtr<RectangleShape> RectangleShapePtr;
};