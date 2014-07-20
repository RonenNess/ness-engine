#include "events_poller.h"

namespace Ness
{
	namespace Utils
	{
		void EventsPoller::poll_events(eventCallback callback, bool callIfHandled)
		{
			// fetch all events we got to fetch
			Event event;
			while( (SDL_PollEvent( &event ) != 0 ))
			{
				// call all handlers
				bool wasHandled = false;
				for (unsigned int i = 0; i < m_handlers.size(); i++)
				{
					wasHandled |= m_handlers[i]->inject_event(event);
				}

				// call the callback
				if (callback)
				{
					if (callIfHandled || (!wasHandled))
					{
						callback(event);
					}
				}
			}
		}
	};
};