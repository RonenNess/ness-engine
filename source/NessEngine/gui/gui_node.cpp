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

#include "gui_node.h"
#include "widgets/widget_api.h"

namespace Ness
{
	namespace Gui
	{
		// add/remove sub entity or node
		void GuiNode::add(const RenderablePtr& object)
		{
			// make sure element is a gui element
			if (!is_gui_element(object))
				throw IllegalAction("Cannot add element that is not a gui element to a GuiNode!");

			// add it
			BaseNode::add(object);
		}

		void GuiNode::add_first(const RenderablePtr& object)
		{
			// make sure element is a gui element
			if (!is_gui_element(object))
				throw IllegalAction("Cannot add element that is not a gui element to a GuiNode!");

			// add it
			BaseNode::add_first(object);
		}

		bool GuiNode::is_gui_element(const RenderablePtr& object)
		{
			return ness_ptr_cast<GuiNode>(object).get() != nullptr || ness_ptr_cast<WidgetAPI>(object).get() != nullptr;
		}
	}
}