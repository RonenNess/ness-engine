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
* mouse wrapper
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../basic_types/point.h"
#include <SDL.h>
#include "event_handler.h"

namespace Ness
{
	namespace Utils
	{

		// different mouse buttons
		enum EMouseButtons
		{
			MOUSE_LEFT = 0,
			MOUSE_RIGHT = 1,
			MOUSE_MIDDLE = 2,
			_MOUSE_BUTTONS_COUNT,
		};

		// wrap mouse functionality for easy mouse controls
		class Mouse : public EventsHandler
		{
		private:
			Pointi m_last_mouse_pos;
			bool m_down[_MOUSE_BUTTONS_COUNT];
			bool m_in_screen;

		public:

			// init the mouse wrapper
			NESSENGINE_API Mouse();

			// update the keyboard with incoming event
			NESSENGINE_API virtual bool inject_event(const Event& event);

			// get mouse position
			NESSENGINE_API const Pointi& position();

			// return if mouse is in screen
			NESSENGINE_API inline bool in_screen() const {return m_in_screen;}

			// get mouse button state
			NESSENGINE_API inline bool is_down(EMouseButtons button) const {return m_down[button];}

		private:

			void change_button_state(int button, bool isDown);
		};

	};
};