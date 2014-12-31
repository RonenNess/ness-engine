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
#include "label.h"

namespace Ness
{
	namespace Gui
	{
		Label::Label(GuiManager* manager, GuiContainerAPI* parent, const String& text) 
				: WidgetAPI(manager, parent)
		{
			// create the text
			m_text = m_parent->get_node()->create_text(manager->get_font(), text);

			// to calculate text position
			__invoke_event_update_position();
		}

		// set container position, relative to parent, in pixels
		void Label::set_position(const Point& new_pos, const Point& anchor)
		{
			m_position = new_pos;
			m_text->set_anchor(anchor);
			__invoke_event_update_position();
		}

		void Label::render()
		{
			m_text->render();
		}

		const BoundingBox& Label::get_bounding_box() const
		{
			static BoundingBox box;
			box = BoundingBox(m_text->get_absolute_position(), m_text->get_absolute_size());
			return box;
		}

		void Label::__invoke_event_update_position()
		{
			m_text->set_position(m_parent->get_absolute_position() + m_position);
		}
	}
}