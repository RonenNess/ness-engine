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
* A scene node for gui elements
* Author: Ronen Ness
* Since: 12/1014
*/

#pragma once
#include "../managed_resources/managed_texture.h"
#include "../renderable/nodes/basic_node.h"
#include "../renderable/transformable_api.h"
#include "../basic_types/containers.h"

namespace Ness
{
	namespace Gui
	{
		// a special scene node to hold and manage gui elements
		class GuiNode : public BaseNode
		{
		protected:

		public:
			NESSENGINE_API GuiNode(Renderer* renderer) : 
				BaseNode(renderer) {}

			// add/remove sub entity or node
			NESSENGINE_API virtual void add(const RenderablePtr& object);
			NESSENGINE_API virtual void add_first(const RenderablePtr& object);

		protected:
			// return true if given pointer is a gui element
			NESSENGINE_API bool is_gui_element(const RenderablePtr& object);
		};

		// a pointer to a gui node
		NESSENGINE_API typedef SharedPtr<GuiNode> GuiNodePtr;
	}
}