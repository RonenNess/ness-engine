/**
* keyboard wrapper
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <SDL.h>
#include <unordered_map>
#include "../exports.h"
#include "event_handler.h"

namespace Ness
{
	namespace Utils
	{

		// wrap keyboard functionality for easy keyboard controls
		class Keyboard : public EventsHandler
		{
		private:
			std::unordered_map<Keycode, bool> m_key_codes;

		public:

			// update the keyboard with incoming event
			NESSENGINE_API virtual bool inject_event(const Event& event);

			// get key state
			NESSENGINE_API inline bool ket_state(Keycode key) {return m_key_codes[key];}
		};

	};
};