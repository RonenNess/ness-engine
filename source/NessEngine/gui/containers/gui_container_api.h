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
* The API of a gui container element, that can contain other widgets and containers
* this is equivalent to a scene node, but for gui.
* Author: Ronen Ness
* Since: 12/1014
*/

#pragma once

#include "../../renderable/nodes/node.h"
#include "../gui_element_api.h"
#include "../../basic_types/containers.h"

namespace Ness
{
	namespace Gui
	{

		// the API of a gui widget that can handle user input events and focus
		class GuiContainerAPI : public GuiElementAPI
		{
		protected:
			Containers::Vector<GuiElementPtr>	m_sons;

		public:

			// create the container
			NESSENGINE_API GuiContainerAPI(GuiManager* manager, GuiContainerAPI* parent) : GuiElementAPI(manager, parent) {}
			
			// add/remove gui element from this container
			NESSENGINE_API virtual void __add(const GuiElementPtr& element);
			NESSENGINE_API virtual void __remove(const GuiElementPtr& element);
			NESSENGINE_API virtual void __add(GuiElementAPI* element);
			NESSENGINE_API virtual void __remove_unsafe(GuiElementAPI* element);
		};

		// a pointer to a gui widget
		NESSENGINE_API typedef SharedPtr<GuiContainerAPI> GuiContainerPtr;
	}
}