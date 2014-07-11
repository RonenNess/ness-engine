/**
* keyboard wrapper
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <SDL.h>
#include <unordered_map>

namespace Ness
{
	namespace Utils
	{

		// wrap keyboard functionality for easy keyboard controls
		class Keyboard
		{
		private:
			std::unordered_map<SDL_Keycode, bool> m_key_codes;

		public:

			// update the keyboard with incoming event
			// always call this, Keyboard will filter it out if it's not a keyboard event!
			void inject_event(const SDL_Event& event)
			{
				switch (event.type)
				{
					case SDL_KEYDOWN:
						m_key_codes[event.key.keysym.sym] = true;
						break;

					case SDL_KEYUP:
						m_key_codes[event.key.keysym.sym] = false;
						break;
				}
			}

			// get key state
			inline bool ket_state(SDL_Keycode key) {return m_key_codes[key];}
		};

	};
};