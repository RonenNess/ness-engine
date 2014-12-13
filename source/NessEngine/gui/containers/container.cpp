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

#include "../gui_manager.h"
#include "container.h"

namespace Ness
{
	namespace Gui
	{
		Container::Container(GuiManager* manager, GuiContainerAPI* parent, const Pointi& size_in_units) 
			: GuiContainerAPI(manager, parent), m_size(size_in_units)
		{
			// create the graphical part
			m_graphics = ness_make_ptr<TileMap>(this->m_manager->renderer(), 
				this->m_manager->get_resources_path() + "frame_unfocused.png", 
				size_in_units, m_manager->get_unit_size());

			// set parts
			Sizei parts_count(3, 3);
			Pointi index;
			for (index.x = 0; index.x < m_size.x; ++index.x)
			{
				for (index.y = 0; index.y < m_size.y; ++index.y)
				{
					SpritePtr tile = m_graphics->get_sprite(index);
					tile->set_blend_mode(BLEND_MODE_BLEND);
					if (index.x == 0)
					{
						if (index.y == 0)
						{
							tile->set_source_from_sprite_sheet(Pointi(0, 0), parts_count);
						}
						else if (index.y == m_size.y-1)
						{
							tile->set_source_from_sprite_sheet(Pointi(0, 2), parts_count);
						}
						else
						{
							tile->set_source_from_sprite_sheet(Pointi(0, 1), parts_count);
						}
					}
					else if (index.y == 0)
					{
						if (index.x == m_size.x-1)
						{
							tile->set_source_from_sprite_sheet(Pointi(2, 0), parts_count);
						}
						else
						{
							tile->set_source_from_sprite_sheet(Pointi(1, 0), parts_count);
						}
					}
					else if (index.x == m_size.x-1)
					{
						if (index.y == m_size.y-1)
						{
							tile->set_source_from_sprite_sheet(Pointi(2, 2), parts_count);
						}
						else
						{
							tile->set_source_from_sprite_sheet(Pointi(2, 1), parts_count);
						}
					}
					else if (index.y == m_size.y-1)
					{
						tile->set_source_from_sprite_sheet(Pointi(1, 2), parts_count);
					}
					else
					{
						tile->set_source_from_sprite_sheet(Pointi(1, 1), parts_count);
					}
				}
			}
		}

		void Container::render()
		{
			m_graphics->render();
		}

		void Container::need_update()
		{
		}

		void Container::invoke_event_get_focus()
		{
		}

		void Container::invoke_event_lose_focus()
		{
		}

		void Container::invoke_event_mouse_enter()
		{
		}

		void Container::invoke_event_mouse_leave()
		{
		}

		bool Container::is_point_on(const Ness::Pointi& pos)
		{
			// get absolute position and size
			const BoundingBox& box = get_bounding_box();

			// check position against bounderies
			if (pos.x >= box.x && pos.y >= box.y &&
				pos.x <= box.x + box.w && pos.y <= box.y + box.h)
				return true;

			return false;
		}

		void Container::invoke_event_click(EMouseButtons mouse_button, const Ness::Pointi& mouse_pos)
		{
			
			// new focused widget
			GuiElementPtr new_focused;

			// loop over son elements and check if we clicked on an element
			// note: go from last to first so that latter widgets will have priority
			for (unsigned int i = m_sons.size(); i > 0; --i)
			{
				GuiElementPtr& widget = m_sons[i];
				if (widget->is_point_on(mouse_pos))
				{
					widget->invoke_event_click(mouse_button, mouse_pos);
					new_focused = widget;
					break;
				}
			}

			// change focus
			if (m_focused_widget)
				m_focused_widget->invoke_event_lose_focus();
			if (new_focused)
				new_focused->invoke_event_get_focus();

			// store the newly focused widget
			m_focused_widget = new_focused;
		}

		void Container::invoke_event_key_down(EMouseButtons key)
		{
			if (m_focused_widget)
				m_focused_widget->invoke_event_key_down(key);
		}

		void Container::invoke_event_key_up(EMouseButtons key)
		{
			if (m_focused_widget)
				m_focused_widget->invoke_event_key_up(key);
		}
	}
  }