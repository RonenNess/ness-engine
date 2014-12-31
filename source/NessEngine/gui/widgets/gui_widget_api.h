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
* The API of a gui widget.
* this is equivalent to a scene entity, but for gui.
* Author: Ronen Ness
* Since: 12/1014
*/

#pragma once

#include "../gui_element_api.h"

namespace Ness
{
	namespace Gui
	{

		// the API of a gui widget
		class WidgetAPI : public GuiElementAPI
		{
		protected:
			bool			m_mouse_inside;		// is mouse currently inside this widget

		public:
			NESSENGINE_API WidgetAPI(GuiManager* manager, GuiContainerAPI* parent) 
				: GuiElementAPI(manager, parent)
			{ }

			// return if mouse is inside the widget
			NESSENGINE_API virtual bool is_mouse_on() {return m_mouse_inside;}

			// default event handling
			NESSENGINE_API virtual void __invoke_event_mouse_enter(const Pointi& mouse_pos) {m_mouse_inside = true;}
			NESSENGINE_API virtual void __invoke_event_mouse_leave(const Pointi& mouse_pos) {m_mouse_inside = false;}
			NESSENGINE_API virtual void __invoke_event_mouse_hover(const Pointi& mouse_pos) {}
			NESSENGINE_API virtual void __invoke_event_update_position() {}
			NESSENGINE_API virtual void __invoke_event_click(EMouseButtons mouse_button, const Pointi& mouse_pos) {}
			NESSENGINE_API virtual void __invoke_event_key_down(EMouseButtons key) {}
			NESSENGINE_API virtual void __invoke_event_key_up(EMouseButtons key) {}
			NESSENGINE_API virtual void __invoke_event_visibility_changed(bool new_state, bool by_parent) {}
			NESSENGINE_API virtual void __invoke_event_enabled_changed(bool new_state, bool by_parent) {}
			NESSENGINE_API virtual void __invoke_event_get_focus() {}
			NESSENGINE_API virtual void __invoke_event_lose_focus() {}
		};

		// a pointer to a gui widget
		NESSENGINE_API typedef SharedPtr<WidgetAPI> WidgetPtr;
	}
}