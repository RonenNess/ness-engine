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
* wrap application events, like minimize, maximize, quit, etc..
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <SDL.h>
#include "../../exports.h"
#include "event_handler.h"

namespace Ness
{
	namespace Utils
	{

		// wrap general application events
		class ApplicationEvents : public EventsHandler
		{
		private:
			bool m_quit;
			bool m_focused;

		public:

			NESSENGINE_API ApplicationEvents() : m_quit(false), m_focused(true) 
			{}

			// update the keyboard with incoming event
			NESSENGINE_API virtual bool inject_event(const Event& event);

			// get if we got certain events
			NESSENGINE_API inline bool got_quit() const {return m_quit;}
			NESSENGINE_API inline bool is_focused() const {return m_focused;}

			// called at the begining of a new frame
			NESSENGINE_API virtual void start_frame() {}
		};

	};
};