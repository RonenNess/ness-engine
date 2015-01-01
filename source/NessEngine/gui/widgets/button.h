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
* A clickable button with text or image on it
* Author: Ronen Ness
* Since: 12/1014
*/

#pragma once

#include "gui_widget_api.h"
#include "label.h"
#include "frame.h"
#include "../../renderable/entities/text.h"

namespace Ness
{
	namespace Gui
	{

		// a clickable button widget
		class Button : public WidgetAPI
		{
		protected:
			LabelPtr	m_text;		// the label part of the button
			FramePtr	m_frame;	// the frame part of the button
			Point		m_position;	// position in pixels relative to parent

		public:

			// create the button
			// text is the text to write on it
			// size_in_grid_units is the size of the button frame, or ZERO to match frame size to the text (default)
			NESSENGINE_API Button(GuiManager* manager, GuiContainerAPI* parent, const String& text, const Sizei& size_in_grid_units = Sizei::ZERO);
			NESSENGINE_API ~Button();			

			// set container position, relative to parent, in pixels
			NESSENGINE_API virtual void set_position(const Point& new_pos, const Point& anchor = Point::ZERO);

			// make the button look invalid
			NESSENGINE_API virtual void __invoke_event_enabled_changed(bool new_state, bool by_parent);

			// recalculate button position when moved
			NESSENGINE_API virtual void __invoke_event_update_position();
			
			// return label bounding box
			NESSENGINE_API virtual const BoundingBox& get_bounding_box() const;

			// set label params
			NESSENGINE_API void change_text(const String& text) {m_text->change_text(text);}
			NESSENGINE_API const String& get_text() {return m_text->get_text();}
			NESSENGINE_API void set_text_color(const Color& color) {m_text->set_color(color);}
			NESSENGINE_API const Color& get_text_color() {return m_text->get_color();}

			// set frame params
			NESSENGINE_API void set_frame_color(const Color& color) {m_frame->set_color(color);}
			NESSENGINE_API const Color& get_frame_color() {return m_frame->get_color();}
		};

		// a pointer to a label widget
		NESSENGINE_API typedef SharedPtr<Button> ButtonPtr;
	}
}