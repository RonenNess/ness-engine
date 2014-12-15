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
* the API for a renderable entity
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "renderable_api.h"

namespace Ness
{
	class NodeAPI;

	// the API of a renderable entity (entities are contained in a node)
	class EntityAPI : public RenderableAPI
	{
	public:
		// create the entity
		NESSENGINE_API EntityAPI(Renderer* renderer) : 
			RenderableAPI(renderer) {}

		// return the last target rectanble (region this entity was rendered upon) without
		// changing the internal state of the entity
		NESSENGINE_API virtual const Rectangle& get_last_target_rect() const = 0;

		// return if this entity touches the given point (based on last target rect)
		NESSENGINE_API virtual bool touch_point(const Pointf& pos) const
		{
			 const Rectangle& rect = get_last_target_rect();
			 return (pos.x >= rect.x && pos.y >= rect.y && pos.x <= rect.x + rect.w && pos.y <= rect.y + rect.h);
		}

		// is it node or entity?
		NESSENGINE_API virtual bool is_node() const {return false;}
		NESSENGINE_API virtual bool is_entity() const {return true;}

	};

	// entity pointer
	NESSENGINE_API typedef SharedPtr<EntityAPI> EntityPtr;
};