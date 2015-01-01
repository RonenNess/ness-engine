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
#include "../../renderer/renderer.h"
#include "button.h"

namespace Ness
{
	namespace Gui
	{
		Button::Button(GuiManager* manager, GuiContainerAPI* parent, const String& text, const Sizei& size_in_grid_units)
				: WidgetAPI(manager, parent)
		{
			// create the text
			m_text = ness_make_ptr<Label>(manager, parent, text);

			// set default size if needed
			Sizei size = size_in_grid_units;
			if (size == Sizei::ZERO)
			{
				size.x = (int)ceil(((float)manager->get_unit_size().x / (float)m_text->get_bounding_box().w));
				size.y = (int)ceil(((float)manager->get_unit_size().y / (float)m_text->get_bounding_box().h));
			}

			// create the frame
			m_frame = ness_make_ptr<Frame>(manager, parent, size, "button");

			// to calculate text position
			set_position(Point::ZERO);
		}

		Button::~Button()
		{
		}

		void Button::__invoke_event_enabled_changed(bool new_state, bool by_parent)
		{
			m_text->__invoke_event_enabled_changed(new_state, by_parent);
			m_frame->__invoke_event_enabled_changed(new_state, by_parent);
		}

		// set container position, relative to parent, in pixels
		void Button::set_position(const Point& new_pos, const Point& anchor)
		{
			m_position = new_pos + Point(m_manager->get_unit_size()) * 0.5f;
			//m_text->set_anchor(anchor);
			__invoke_event_update_position();
		}

		const BoundingBox& Button::get_bounding_box() const
		{
			static BoundingBox box;
			//box = BoundingBox(m_text->get_absolute_position(), m_text->get_absolute_size());
			return box;
		}

		void Button::__invoke_event_update_position()
		{
			m_text->set_position(m_position);
		}
	}
}