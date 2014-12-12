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

#include "frame.h"

namespace Ness
{
	namespace Gui
	{
		Frame::Frame(Renderer* renderer, const Pointi& size) : GuiNode(renderer), m_size(size)
		{
			// create the border parts
			m_graphics = ness_make_ptr<TileMap>(this->m_renderer, "frame_unfocused.png", mapSize, singleTileSize, tilesDistance);
		}


		void Frame::invoke_event_get_focus()
		{
		}

		void Frame::invoke_event_lose_focus()
		{
		}

		bool Frame::is_point_on(const Ness::Pointi& pos)
		{
			// get absolute position and size
			const SRenderTransformations& trans = get_absolute_transformations();

			// check position against bounderies
			if (pos.x >= trans.position.x && pos.y >= trans.position.y &&
				pos.x <= trans.position.x + (m_size.x * trans.scale.x) && pos.y <= trans.position.y + (m_size.y * trans.scale.y))
				return true;

			return false;
		}

		void Frame::invoke_event_click(EMouseButtons mouse_button, const Ness::Pointi& mouse_pos)
		{
			
			// new focused widget
			WidgetPtr new_focused;

			// loop over son elements and check if we clicked on an element
			// note: go from last to first so that latter widgets will have priority
			for (unsigned int i = m_entities.size(); i > 0; --i)
			{
				WidgetPtr widget = ness_ptr_cast<WidgetAPI>(m_entities[i]);
				if (widget && widget->is_point_on(mouse_pos))
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

		void Frame::invoke_event_key_down(EMouseButtons key)
		{
			if (m_focused_widget)
				m_focused_widget->invoke_event_key_down(key);
		}

		void Frame::invoke_event_key_up(EMouseButtons key)
		{
			if (m_focused_widget)
				m_focused_widget->invoke_event_key_up(key);
		}
	}
  }