#include "keyboard.h"

namespace Ness
{
	namespace Utils
	{

		// update the keyboard with incoming event
		// always call this, Keyboard will filter it out if it's not a keyboard event!
		bool Keyboard::inject_event(const Event& event)
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					m_key_codes[event.key.keysym.sym] = true;
					return true;

				case SDL_KEYUP:
					m_key_codes[event.key.keysym.sym] = false;
					return true;
			}
			return false;
		}
	};
};