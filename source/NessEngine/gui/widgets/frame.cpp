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
#include "frame.h"

namespace Ness
{
	namespace Gui
	{
		Frame::Frame(GuiManager* manager, GuiContainerAPI* parent, const Pointi& size_in_units, const String& texture_prefix) 
			: WidgetAPI(manager, parent), m_textures_prefix(texture_prefix), m_size(size_in_units), 
			m_mouse_inside(false), m_is_focused(false)
		{
			// calculate bounding box
			calculate_bounding_box();

			// create the frame graphics
			{
				// create the graphical part
				m_graphics = m_parent->get_node()->create_tilemap(this->m_manager->get_resources_path() + m_textures_prefix + "_unfocused.png", size_in_units, m_manager->get_unit_size());

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

		void Frame::calculate_bounding_box()
		{
			if (m_parent)
			{
				m_bounding_box.x = m_parent->get_absolute_position().x + (int)m_position.x;
				m_bounding_box.y = m_parent->get_absolute_position().y + (int)m_position.y;
			}
			m_bounding_box.w = m_size.x * m_manager->get_unit_size().x;
			m_bounding_box.h = m_size.y * m_manager->get_unit_size().y;
		}

		void Frame::__invoke_event_update_position()
		{
			// recalculate bounding box
			calculate_bounding_box();

			// update node position
			m_graphics->set_position(m_parent->get_absolute_position() + m_position);
		}

		void Frame::set_position(const Point& new_pos, const Point& anchor)
		{
			// get absolute ew position and if not changed skip
			Ness::Point new_pos_abs = new_pos - Point(m_bounding_box.w * anchor.x, m_bounding_box.h * anchor.y);
			if (m_position == new_pos_abs)
				return;

			// set position and invoke position update
			m_position = new_pos_abs;
			__invoke_event_update_position();
		}

		void Frame::update_texture(const String& state)
		{
			Pointi index;
			for (index.x = 0; index.x < m_size.x; ++index.x)
			{
				for (index.y = 0; index.y < m_size.y; ++index.y)
				{
					SpritePtr& tile = m_graphics->get_sprite(index);
					tile->change_texture(this->m_manager->get_resources_path() + m_textures_prefix + "_" + state + ".png", false);
				}
			}
		}

		void Frame::__invoke_event_get_focus()
		{
			m_is_focused = true;
			update_texture("focused");
		}

		void Frame::__invoke_event_mouse_pressed(EMouseButtons button, const Pointi& mouse_pos)
		{
			update_texture("mouse_down");
		}

		void Frame::__invoke_event_mouse_released(EMouseButtons button, const Pointi& mouse_pos)
		{
			update_texture("focused");
		}

		void Frame::__invoke_event_lose_focus()
		{
			m_is_focused = false;
			update_texture("unfocused");
		}

		void Frame::__invoke_event_mouse_enter(const Pointi& mouse_pos)
		{
			m_mouse_inside = true;
			update_texture("mouse_hover");
		}

		void Frame::__invoke_event_mouse_leave(const Pointi& mouse_pos)
		{
			m_mouse_inside = false;
			
			if (m_is_focused)
			{
				update_texture("focused");
			}
			else
			{
				update_texture("unfocused");
			}
		}

		void Frame::__invoke_event_mouse_hover(const Pointi& mouse_pos)
		{
			m_mouse_inside = true;
		}

		void Frame::__invoke_event_visibility_changed(bool new_state, bool by_parent)
		{
			if (!by_parent)
			{
				m_graphics->set_visible(new_state);
			}
		}

		void Frame::__invoke_event_enabled_changed(bool new_state, bool by_parent)
		{
			if (new_state)
			{
				update_texture("unfocused");
			}
			else
			{
				update_texture("disabled");
			}
		}
	}
  }