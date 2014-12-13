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
		Container::Container(GuiManager* manager, GuiContainerAPI* parent, const Pointi& size_in_units, bool visual) 
			: GuiContainerAPI(manager, parent), m_size(size_in_units), m_mouse_inside(false), m_is_focused(false)
		{
			// set the bounding box
			m_bounding_box.w = m_size.x * m_manager->get_unit_size().x;
			m_bounding_box.h = m_size.y * m_manager->get_unit_size().y;

			// create the container graphics
			if (visual)
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
						SpritePtr& tile = m_graphics->get_sprite(index);
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
		}

		SharedPtr<Container> Container::create_container(const Pointi& size_in_units)
		{
			ContainerPtr ret = ness_make_ptr<Container>(this->m_manager, this, size_in_units);
			add(ret);
			return ret;
		}

		void Container::render()
		{
			// if not visible skip
			if (!m_visible)
				return;

			// render this container
			if (m_graphics)
				m_graphics->render();

			// render son elements
			for (unsigned int i = 0; i < m_sons.size(); ++i)
			{
				m_sons[i]->render();
			}
		}

		void Container::invoke_event_get_focus()
		{
			m_is_focused = true;
			if (m_graphics)
			{
				Pointi index;
				for (index.x = 0; index.x < m_size.x; ++index.x)
				{
					for (index.y = 0; index.y < m_size.y; ++index.y)
					{
						SpritePtr& tile = m_graphics->get_sprite(index);
						tile->change_texture(this->m_manager->get_resources_path() + "frame_focused.png", false);
					}
				}
			}
		}

		void Container::invoke_event_lose_focus()
		{
			m_is_focused = false;
			if (m_graphics)
			{
				Pointi index;
				for (index.x = 0; index.x < m_size.x; ++index.x)
				{
					for (index.y = 0; index.y < m_size.y; ++index.y)
					{
						SpritePtr& tile = m_graphics->get_sprite(index);
						tile->change_texture(this->m_manager->get_resources_path() + "frame_unfocused.png", false);
					}
				}
			}
		}

		void Container::invoke_event_mouse_enter(const Pointi& mouse_pos)
		{
			m_mouse_inside = true;
			if (m_graphics)
			{
				Pointi index;
				for (index.x = 0; index.x < m_size.x; ++index.x)
				{
					for (index.y = 0; index.y < m_size.y; ++index.y)
					{
						SpritePtr& tile = m_graphics->get_sprite(index);
						tile->change_texture(this->m_manager->get_resources_path() + "frame_mouse_hover.png", false);
					}
				}
			}
		}

		void Container::invoke_event_mouse_leave(const Pointi& mouse_pos)
		{
			m_mouse_inside = false;
			if (m_graphics)
			{
				Pointi index;
				for (index.x = 0; index.x < m_size.x; ++index.x)
				{
					for (index.y = 0; index.y < m_size.y; ++index.y)
					{
						SpritePtr& tile = m_graphics->get_sprite(index);
						if (m_is_focused)
							tile->change_texture(this->m_manager->get_resources_path() + "frame_focused.png", false);
						else
							tile->change_texture(this->m_manager->get_resources_path() + "frame_unfocused.png", false);
					}
				}
			}
		}

		void Container::invoke_event_mouse_hover(const Pointi& mouse_pos)
		{
			m_mouse_inside = true;

			// loop over all the sub entities
			for (unsigned int i = 0; i < m_sons.size(); ++i)
			{
				// get current element and check if mouse is inside bounderies
				GuiElementPtr& curr = m_sons[i];

				// skip if disabled or invisible
				if (!curr->is_enabled() || !curr->is_visible())
				{
					continue;
				}

				if (curr->is_point_on(mouse_pos))
				{

					// invoke mouse enter event
					if (!curr->is_mouse_on())
					{
						curr->invoke_event_mouse_enter(mouse_pos);
					}

					// invoke mouse hover event
					curr->invoke_event_mouse_hover(mouse_pos);
				}
				// if mouse is not on this element
				else
				{
					// invoke mouse leave event
					if (curr->is_mouse_on())
					{
						curr->invoke_event_mouse_leave(mouse_pos);
					}
				}
			}
		}

		void Container::invoke_event_visibility_changed(bool newState)
		{
		}

		void Container::invoke_event_enabled_changed(bool newState)
		{
			if (m_graphics)
			{
				Pointi index;
				for (index.x = 0; index.x < m_size.x; ++index.x)
				{
					for (index.y = 0; index.y < m_size.y; ++index.y)
					{
						SpritePtr& tile = m_graphics->get_sprite(index);
						if (newState)
						{
							tile->change_texture(this->m_manager->get_resources_path() + "frame_unfocused.png", false);
						}
						else
						{
							tile->change_texture(this->m_manager->get_resources_path() + "frame_disabled.png", false);
						}
					}
				}
			}
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
			for (unsigned int i = 0; i < m_sons.size(); ++i)
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