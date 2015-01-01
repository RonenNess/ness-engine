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

#include "gui_element_api.h"
#include "containers/gui_container_api.h"
#include "gui_manager.h"

namespace Ness
{
	namespace Gui
	{
		void GuiElementAPI::destroy()
		{
			if (m_parent)
			{
				m_parent->__remove_unsafe(this);
			}
			m_node->remove_from_parent();
		}

		GuiElementAPI::GuiElementAPI(GuiManager* manager, GuiContainerAPI* parent) 
				: m_manager(manager), m_parent(parent), m_visible(true), m_enabled(true) 
		{
			m_node = m_parent->get_node()->create_node();
		}

		bool GuiElementAPI::is_point_on(const Pointi& pos)
		{
			const BoundingBox& box = get_bounding_box();
			return (pos.x >= box.x && pos.x <= box.x + box.w && pos.y >= box.y && pos.y <= box.y + box.h);
		}

		Pointi GuiElementAPI::get_absolute_position() const 
		{
			return Pointi(get_bounding_box().x, get_bounding_box().y);
		}

		void GuiElementAPI::set_position_aligned(const Pointi& new_pos_grid, const Point& anchor)
		{
			set_position(new_pos_grid * m_manager->get_unit_size(), anchor);
		}
	}
}