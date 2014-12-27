#include "application_events.h"

namespace Ness
{
	namespace Utils
	{
		// check relevant events
		bool ApplicationEvents::inject_event(const Event& event)
		{
			switch (event.type)
			{
			case SDL_QUIT:
			case SDL_APP_TERMINATING:
				m_quit = true;
				return true;

			case SDL_WINDOWEVENT:
				switch (event.window.event)
				{
				case SDL_WINDOWEVENT_CLOSE:
					m_quit = true;
					return true;

				case SDL_WINDOWEVENT_FOCUS_GAINED:
					m_focused = true;
					return true;

				case SDL_WINDOWEVENT_FOCUS_LOST:
					m_focused = false;
					return true;
				}
			}
			return false;
		}
	};
};