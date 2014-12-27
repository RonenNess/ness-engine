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
* Object that poll events and distribute them to the handlers
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <SDL.h>
#include "../../basic_types/containers.h"
#include "../../exports.h"
#include "event_handler.h"

namespace Ness
{
	namespace Utils
	{
		// callback function for events polling
		NESSENGINE_API typedef void (*eventCallback)(const Event& event);

		// class that manage polling events and distribute them to the handlers
		class EventsPoller
		{
		private:
			Containers::Vector<EventsHandler*> m_handlers;

		public:

			// add an event handler to the events poller
			NESSENGINE_API inline void add_handler(EventsHandler& handler) {m_handlers.push_back(&handler);}

			// poll all events.
			// callback - optional custom callback function you may provide to handle events as well
			// callIfHandled - if true, will always call the provided callback function. if false, will call callback only if its unhandled event
			NESSENGINE_API void poll_events(eventCallback callback = nullptr, bool callIfHandled = true);
		};
	};
};