/**
* Object that poll events and distribute them to the handlers
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <SDL.h>
#include "../basic_types/containers.h"
#include "../exports.h"
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
			Vector<EventsHandler*> m_handlers;

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