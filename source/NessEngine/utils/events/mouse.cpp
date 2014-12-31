#include "mouse.h"
#include <string.h>

namespace Ness
{
	namespace Utils
	{
		NESSENGINE_API Mouse::Mouse()
		{
			memset(m_down, false, sizeof(m_down));
			m_in_screen = false;
		}

		bool Mouse::inject_event(const Event& event)
		{
			switch (event.type)
			{
				case SDL_MOUSEBUTTONDOWN:
					change_button_state(event.button.button, true);
					return true;

				case SDL_MOUSEBUTTONUP:
					change_button_state(event.button.button, false);
					return true;

					
				case SDL_WINDOWEVENT:
					switch (event.window.event)
					{
					case SDL_WINDOWEVENT_ENTER:
						m_in_screen = true;
						return true;

					case SDL_WINDOWEVENT_LEAVE:
						m_in_screen = false;
						return true;
					}
			}
			return false;
		}

		const Pointi& Mouse::position() 
		{
			SDL_GetMouseState(&m_last_mouse_pos.x, &m_last_mouse_pos.y);
			return m_last_mouse_pos;
		}

		void Mouse::change_button_state(int button, bool isDown)
		{
			switch (button)
			{
			case SDL_BUTTON_LEFT:
				m_down[MOUSE_LEFT] = isDown;
				break;

			case SDL_BUTTON_MIDDLE:
				m_down[MOUSE_MIDDLE] = isDown;
				break;

			case SDL_BUTTON_RIGHT:
				m_down[MOUSE_RIGHT] = isDown;
				break;
			}
		}

	};
};