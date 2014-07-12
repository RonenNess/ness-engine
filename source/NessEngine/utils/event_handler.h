/**
* the API of an object capable of digesting SDL events
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <SDL.h>
#include "../exports.h"

namespace Ness
{
	namespace Utils
	{
		class EventsHandler
		{
		public:
			// the function that gives the event handler an event to examine.
			// should return true if handled the event, false if irrelevant to it.
			NESSENGINE_API virtual bool inject_event(const SDL_Event& event) = 0;
		};
	};
};