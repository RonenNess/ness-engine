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
* The base API of every gui widget. define events a widget should handle
* as well as other basic gui widget functionality.
* Author: Ronen Ness
* Since: 12/1014
*/

#pragma once

#include "../../exports.h"
#include "../../basic_types/events.h"
#include "../../basic_types/point.h"

namespace Ness
{
	namespace Gui
	{

		// the API of a gui widget that can handle user input events and focus
		class WidgetAPI
		{
		public:

			// called when this element get focus
			NESSENGINE_API virtual void invoke_event_get_focus() = 0;

			// called when this element lose focus
			NESSENGINE_API virtual void invoke_event_lose_focus() = 0;

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
		};

		// a pointer to a gui widget
		NESSENGINE_API typedef SharedPtr<WidgetAPI> WidgetPtr;
	}
}