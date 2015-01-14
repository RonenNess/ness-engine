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
* keyboard wrapper
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <SDL.h>
#include "../../exports.h"
#include "event_handler.h"
#include "../../basic_types/containers.h"

namespace Ness
{
	namespace Utils
	{

		// wrap keyboard functionality for easy keyboard controls
		class Keyboard : public EventsHandler
		{
		private:
			unsigned int									m_frame_id;				// frame id of this specific keyboard events handler
			Containers::UnorderedMap<Keycode, bool>			m_key_codes;			// the state (up or down) of all the keyboard keys
			Containers::UnorderedMap<Keycode, unsigned int>	m_frame_of_down;		// the frame id in which the botton was pressed
			Containers::UnorderedMap<Keycode, unsigned int>	m_frame_of_released;	// the frame id in which the botton was released

		public:

			NESSENGINE_API Keyboard() : m_frame_id(0) {}

			// update the keyboard with incoming event
			NESSENGINE_API virtual bool inject_event(const Event& event);

			// get key state
			NESSENGINE_API inline bool key_state(Keycode key) {return m_key_codes[key];}

			// the same is key_state(), wanted more unified API with the mouse.
			NESSENGINE_API inline bool is_down(Keycode key) {return m_key_codes[key];}

			// get if mouse was pressed in this very frame
			NESSENGINE_API inline bool was_pressed(Keycode key) {return m_frame_of_down[key] == m_frame_id;}

			// get if mouse was released in this very frame
			NESSENGINE_API inline bool was_released(Keycode key) {return m_frame_of_released[key] == m_frame_id;}

			// get if a key was clicked in this very frame (meaning a fast pressed and release)
			// threshold_in_seconds is maximum time that can pass since the mouse button was pressed for it to be counted as a click.
			NESSENGINE_API bool was_clicked(Keycode key, float threshold_in_seconds = 0.5f);

			// called at the begining of a new frame
			NESSENGINE_API virtual void start_frame();
		};

	};
};