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

#include "../widgets/all_widgets.h"
#include "../gui_manager.h"
#include "../../renderer/renderer.h"
#include "container.h"

namespace Ness
{
	namespace Gui
	{
		Container::Container(GuiManager* manager, GuiContainerAPI* parent, const Pointi& size_in_units, bool visual) 
			: GuiContainerAPI(manager, parent), m_size(size_in_units), m_mouse_inside(false), m_is_focused(false), m_docking(DOCK_DISABLED)
		{
			// calculate bounding box
			calculate_bounding_box();

			// create the node for this container
			// special case for root container
			if (parent == nullptr)
			{
				m_node = ness_make_ptr<Node>(manager->renderer());
				m_node->set_name("gui_root_container");
			}
			// else create node normally
			else
			{
				m_node = m_parent->get_node()->create_node();
				m_node->set_name("gui_container");
			}

			// create the container graphics
			if (visual)
			{
				// create the graphical part
				m_graphics = m_node->create_tilemap(this->m_manager->get_resources_path() + "frame_unfocused.png", size_in_units, m_manager->get_unit_size());

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

		void Container::calculate_bounding_box()
		{
			if (m_parent)
			{
				m_bounding_box.x = m_parent->get_absolute_position().x + (int)m_position.x;
				m_bounding_box.y = m_parent->get_absolute_position().y + (int)m_position.y;
			}
			m_bounding_box.w = m_size.x * m_manager->get_unit_size().x;
			m_bounding_box.h = m_size.y * m_manager->get_unit_size().y;
		}

		SharedPtr<Container> Container::create_container(const Pointi& size_in_units)
		{
			ContainerPtr ret = ness_make_ptr<Container>(this->m_manager, this, size_in_units);
			add(ret);
			return ret;
		}

		void Container::fix_docking()
		{

			// check if need to update position based on docking
			if (m_docking != DOCK_DISABLED)
			{
				const BoundingBox& parent_box = m_parent->get_bounding_box();
				const Pointi parent_box_center(parent_box.x + (int)(parent_box.w * 0.5f), parent_box.y + (int)(parent_box.h * 0.5f));
				const Pointi parent_box_bottom_right(parent_box.x + parent_box.w, parent_box.y + parent_box.h);
				const Pointi half_size_in_pixels((int)(m_bounding_box.w * 0.5f), (int)(m_bounding_box.h * 0.5f));
				switch (m_docking)
				{
					case DOCK_TOP_CENTER:
						set_position(Pointi(parent_box_center.x - half_size_in_pixels.x, parent_box.y));
						break;

					case DOCK_TOP_LEFT:
						set_position(Pointi(parent_box.x, parent_box.y));
						break;

					case DOCK_TOP_RIGHT:
						set_position(Pointi(parent_box_bottom_right.x - m_bounding_box.w, parent_box.y));
						break;

					case DOCK_BOTTOM_CENTER:
						set_position(Pointi(parent_box_center.x - half_size_in_pixels.x, parent_box_bottom_right.y - m_bounding_box.h));
						break;

					case DOCK_BOTTOM_LEFT:
						set_position(Pointi(parent_box.x, parent_box_bottom_right.y - m_bounding_box.h));
						break;

					case DOCK_BOTTOM_RIGHT:
						set_position(Pointi(parent_box_bottom_right.x - m_bounding_box.w, parent_box_bottom_right.y - m_bounding_box.h));
						break;

					case DOCK_LEFT_CENTER:
						set_position(Pointi(parent_box.x, parent_box_center.y - half_size_in_pixels.y));
						break;

					case DOCK_RIGHT_CENTER:
						set_position(Pointi(parent_box_bottom_right.x - m_bounding_box.w, parent_box_center.y - half_size_in_pixels.y));
						break;

					case DOCK_CENTER:
						set_position(parent_box_center - half_size_in_pixels);
						break;
				}
			}
		}

		LabelPtr Container::create_label(const String& text)
		{
			LabelPtr ret = ness_make_ptr<Label>(this->m_manager, this, text);
			add(ret);
			return ret;
		}

		void Container::__invoke_event_update_position()
		{
			// update docking
			fix_docking();

			// recalculate bounding box
			calculate_bounding_box();

			// update node position
			m_node->set_position(m_parent->get_absolute_position() + m_position);

			// notify all son entities
			for (unsigned int i = 0; i < m_sons.size(); ++i)
			{
				m_sons[i]->__invoke_event_parent_moved();
			}
		}

		void Container::set_position(const Point& new_pos, const Point& anchor)
		{
			// get absolute ew position and if not changed skip
			Ness::Point new_pos_abs = new_pos - Point(m_bounding_box.w * anchor.x, m_bounding_box.h * anchor.y);
			if (m_position == new_pos_abs)
				return;

			// set position and invoke position update
			m_position = new_pos_abs;
			__invoke_event_update_position();
		}

		void Container::__invoke_event_get_focus()
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

		void Container::__invoke_event_lose_focus()
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

		void Container::__invoke_event_mouse_enter(const Pointi& mouse_pos)
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

		void Container::__invoke_event_mouse_leave(const Pointi& mouse_pos)
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

		void Container::__invoke_event_mouse_hover(const Pointi& mouse_pos)
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
						curr->__invoke_event_mouse_enter(mouse_pos);
					}

					// invoke mouse hover event
					curr->__invoke_event_mouse_hover(mouse_pos);
				}
				// if mouse is not on this element
				else
				{
					// invoke mouse leave event
					if (curr->is_mouse_on())
					{
						curr->__invoke_event_mouse_leave(mouse_pos);
					}
				}
			}
		}

		void Container::__invoke_event_visibility_changed(bool new_state, bool by_parent)
		{
			// set the node visibility
			if (!by_parent)
			{
				m_node->set_visible(new_state);
			}

			// invoke event for all sub entities
			for (unsigned int i = 0; i < m_sons.size(); ++i)
			{
				m_sons[i]->__invoke_event_visibility_changed(new_state, true);
			}
		}

		void Container::__invoke_event_enabled_changed(bool new_state, bool by_parent)
		{
			if (m_graphics)
			{
				Pointi index;
				for (index.x = 0; index.x < m_size.x; ++index.x)
				{
					for (index.y = 0; index.y < m_size.y; ++index.y)
					{
						SpritePtr& tile = m_graphics->get_sprite(index);
						if (new_state)
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
			for (unsigned int i = 0; i < m_sons.size(); ++i)
			{
				m_sons[i]->__invoke_event_enabled_changed(new_state, true);
			}
		}

		void Container::__invoke_event_click(EMouseButtons mouse_button, const Pointi& mouse_pos)
		{
			
			// new focused widget
			GuiElementPtr new_focused;

			// loop over son elements and check if we clicked on an element
			for (unsigned int i = 0; i < m_sons.size(); ++i)
			{
				GuiElementPtr& widget = m_sons[i];
				if (widget->is_point_on(mouse_pos))
				{
					widget->__invoke_event_click(mouse_button, mouse_pos);
					new_focused = widget;
					break;
				}
			}

			// change focus
			if (m_focused_widget)
				m_focused_widget->__invoke_event_lose_focus();
			if (new_focused)
				new_focused->__invoke_event_get_focus();

			// store the newly focused widget
			m_focused_widget = new_focused;
		}


		void Container::__invoke_event_key_down(EMouseButtons key)
		{
			if (m_focused_widget)
				m_focused_widget->__invoke_event_key_down(key);
		}

		void Container::__invoke_event_key_up(EMouseButtons key)
		{
			if (m_focused_widget)
				m_focused_widget->__invoke_event_key_up(key);
		}
	}
  }