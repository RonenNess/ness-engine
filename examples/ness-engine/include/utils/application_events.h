/**
* wrap application events, like minimize, maximize, quit, etc..
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <SDL.h>
#include "../exports.h"
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
		};

	};
};