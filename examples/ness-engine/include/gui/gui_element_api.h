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
* The base API of every gui element, be it a widget or a container. 
* Author: Ronen Ness
* Since: 12/1014
*/

#pragma once

#include "../exports.h"
#include "../basic_types/events.h"
#include "../basic_types/point.h"
#include "../basic_types/pointers.h"
#include "../basic_types/rectangle.h"

namespace Ness
{
	namespace Gui
	{
		// predeclare the gui manager
		class GuiManager;
		class GuiContainerAPI;

		// gui element bounding box
		NESSENGINE_API typedef Rectangle BoundingBox;

		// the API of a gui widget that can handle user input events and focus
		class GuiElementAPI
		{
		protected:
			GuiManager*			m_manager;		// pointer to the gui manager (the root object)
			GuiContainerAPI*	m_parent;		// pointer to the gui element containing this element
			bool				m_visible;		// is this element visible?
			bool				m_enabled;		// is this element enabled?

		public:

			NESSENGINE_API GuiElementAPI(GuiManager* manager, GuiContainerAPI* parent) 
				: m_manager(manager), m_parent(parent), m_visible(true), m_enabled(true) 
			{}

			// set/get enabled
			NESSENGINE_API inline void set_enabled(bool enabled) {if (m_enabled == enabled) return; m_enabled = enabled; need_update();}
			NESSENGINE_API inline bool is_enabled() const {return m_enabled;}

			// set/get visible
			NESSENGINE_API inline void set_visible(bool visible) {if (m_visible == visible) return; m_visible = visible; need_update();}
			NESSENGINE_API inline bool is_visible() const {return m_visible;}

			// remove this gui element
			NESSENGINE_API virtual void remove_from_parent();

			// called when this element needs to be updated (when state changes)
			NESSENGINE_API virtual void need_update() = 0;

			// called when this element get focus
			NESSENGINE_API virtual void invoke_event_get_focus() = 0;

			// called when this element lose focus
			NESSENGINE_API virtual void invoke_event_lose_focus() = 0;

			// called when mouse enter the element bounderies
			NESSENGINE_API virtual void invoke_event_mouse_enter() = 0;

			// called when mouse leave the element bounderies
			NESSENGINE_API virtual void invoke_event_mouse_leave() = 0;

			// return the element's bounding box
			// NOTE: the returned bounding box needs to be absolute, i.e. absolute position
			// in pixels with absolute height/width in pixels.
			NESSENGINE_API virtual const BoundingBox& get_bounding_box() = 0;

			// invoke mouse click event on this element
			// mouse_button is the button pressed (left / right / middle mouse click)
			// mouse_pos is absolute mouse position
			NESSENGINE_API virtual void invoke_event_click(EMouseButtons mouse_button, const Ness::Pointi& mouse_pos) = 0;

			// invoke key-down event (this event should be called when this element is focused)
			NESSENGINE_API virtual void invoke_event_key_down(EMouseButtons key) = 0;

			// invoke key-down event (this event should be called when this element is focused)
			NESSENGINE_API virtual void invoke_event_key_up(EMouseButtons key) = 0;

			// is given absolute position within the bounderies of this widget?
			NESSENGINE_API virtual bool is_point_on(const Ness::Pointi& pos) = 0;

			// render the element!
			NESSENGINE_API virtual void render() = 0;
		};

		// a pointer to a gui widget
		NESSENGINE_API typedef SharedPtr<GuiElementAPI> GuiElementPtr;
	}
}