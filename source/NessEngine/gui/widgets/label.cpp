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
			m_text = m_node->create_text(manager->get_font(), text);
			m_text->set_anchor(Point::ZERO);
			m_text->set_blend_mode(BLEND_MODE_BLEND);
			m_text->set_color(manager->get_default_text_color());
			m_text->set_scale(0.5f);

			// set shadow params
			set_shadow(m_manager->get_default_text_shadow_color(), m_manager->get_default_text_shadow_offset());

			// to calculate text position
			set_position(Point::ZERO);
		}

		Label::~Label()
		{
			m_text->remove_from_parent();
		}

		void Label::set_shadow(const Color& shadow_color, const Pointi& offset)
		{
			m_text->set_shadow(shadow_color, offset);
		}

		void Label::set_background_image(const String& texture_file, bool fit_text_size)
		{
			m_back_img = m_node->create_sprite(texture_file);
			m_back_img->set_blend_mode(BLEND_MODE_BLEND);
			m_node->remove(m_back_img);
			m_node->add_first(m_back_img);
			if (fit_text_size)
			{
				m_back_img->set_position(m_text->get_position());
				m_back_img->set_size(m_text->get_size());
				m_back_img->set_scale(m_text->get_scale());
				m_back_img->set_anchor(m_text->get_anchor());
			}
			else
			{
				m_back_img->set_anchor(Ness::Point::HALF);
				m_back_img->set_position(m_text->get_position() + m_text->get_absolute_size() * m_text->get_anchor());
			}
		}
		
		void Label::clear_background_image()
		{
			m_back_img->remove_from_parent();
			m_back_img.reset();
		}

		void Label::disable_shadow()
		{
			m_text->disable_shadow();
		}

		void Label::__invoke_event_enabled_changed(bool new_state, bool by_parent)
		{
			if (new_state)
			{
				m_text->set_opacity(1.0f);
			}
			else
			{
				m_text->set_opacity(0.5f);
			}
		}

		void Label::set_position(const Point& new_pos, const Point& anchor)
		{
			m_position = new_pos + m_manager->get_padding_size();
			m_text->set_anchor(anchor);
			__invoke_event_update_position();
		}

		void Label::set_alignment(ETextAlignment alignment)
		{
			switch (alignment)
			{
			case TEXT_ALIGN_LEFT:
				set_position(Point(0.0f, m_text->get_position().y - m_manager->get_padding_size().y), Point(0.0f, 0.0f));
				break;

			case TEXT_ALIGN_RIGHT:
				set_position(Point((float)(m_parent->get_bounding_box().w - m_manager->get_padding_size().x), 
					(float)m_text->get_position().y - m_manager->get_padding_size().y), Point(1.0f, 0.0f));
				break;

			case TEXT_ALIGN_CENTER:
				set_position(Point((float)(m_parent->get_bounding_box().w / 2 - m_manager->get_padding_size().x), 
					(float)m_text->get_position().y - m_manager->get_padding_size().y), Point(0.5f, 0.0f));
				break;
			}
		}

		const BoundingBox& Label::get_bounding_box() const
		{
			static BoundingBox box;
			box = BoundingBox(m_text->get_absolute_position(), m_text->get_absolute_size());
			return box;
		}

		void Label::__invoke_event_update_position()
		{
			m_text->set_position(m_position);
		}
	}
}