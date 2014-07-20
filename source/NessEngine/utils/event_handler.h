/**
* the API of an object capable of digesting SDL events
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <SDL.h>
#include "../exports.h"
#include "../basic_types/events.h"

namespace Ness
{
	namespace Utils
	{
		class EventsHandler
		{
		public:
			// the function that gives the event handler an event to examine.
			// should return true if handled the event, false if irrelevant to it.
			NESSENGINE_API virtual bool inject_event(const Event& event) = 0;
		};
	};
};