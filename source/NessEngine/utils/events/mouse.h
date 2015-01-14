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
#include "../../basic_types/point.h"
#include <SDL.h>
#include "event_handler.h"

namespace Ness
{
	namespace Utils
	{

		// wrap mouse functionality for easy mouse controls
		class Mouse : public EventsHandler
		{
		private:
			Pointi			m_last_mouse_pos;							// last mouse position
			unsigned int	m_frame_id;									// frame id of this specific mouse events handler
			bool			m_down[_MOUSE_BUTTONS_COUNT];				// mouse keys state
			unsigned int	m_frame_of_down[_MOUSE_BUTTONS_COUNT];		// the frame id in which mouse keys were last pressed down
			unsigned int	m_frame_of_released[_MOUSE_BUTTONS_COUNT];	// the frame id in which mouse keys were last released
			bool			m_in_screen;								// is the mouse currently inside the window of the app

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
			NESSENGINE_API inline bool is_down(EMouseButtons button = MOUSE_LEFT) const {return m_down[button];}

			// get if mouse was pressed in this very frame
			NESSENGINE_API inline bool was_pressed(EMouseButtons button = MOUSE_LEFT) const {return m_frame_of_down[button] == m_frame_id;}

			// get if mouse was released in this very frame
			NESSENGINE_API inline bool was_released(EMouseButtons button = MOUSE_LEFT) const {return m_frame_of_released[button] == m_frame_id;}

			// end frame
			NESSENGINE_API virtual void end_frame() {m_frame_id++;}

			// get if mouse was clicked in this very frame (meaning a fast pressed and released
			NESSENGINE_API bool was_clicked(EMouseButtons button = MOUSE_LEFT) const;

		private:

			void change_button_state(int button, bool isDown);
		};

	};
};