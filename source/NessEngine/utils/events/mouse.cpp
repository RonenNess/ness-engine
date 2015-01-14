#include "mouse.h"
#include <string.h>

namespace Ness
{
	namespace Utils
	{
		NESSENGINE_API Mouse::Mouse()
		{
			memset(m_down, false, sizeof(m_down));
			memset(m_frame_of_down, -1, sizeof(m_frame_of_down));
			memset(m_frame_of_released, -1, sizeof(m_frame_of_released));
			m_frame_id = 0;
			m_in_screen = false;
		}

		bool Mouse::inject_event(const Event& event)
		{
			switch (event.type)
			{
				// mouse button pressed
				case SDL_MOUSEBUTTONDOWN:
					change_button_state(event.button.button, true);
					return true;

				// mouse button released
				case SDL_MOUSEBUTTONUP:
					change_button_state(event.button.button, false);
					return true;

				// mouse leave / enter window
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

		bool Mouse::was_clicked(EMouseButtons button) const 
		{
			return (m_frame_of_released[button] == m_frame_id && m_frame_of_down[button] + 30 <= m_frame_id);
		}

		void Mouse::change_button_state(int button, bool isDown)
		{
			// if no change, skip
			if (m_down[button] == isDown)
				return;

			// set state
			m_down[button] = isDown;

			// set the frame in which the state changed
			if (isDown) 
			{
				m_frame_of_down[button] = m_frame_id; 
			}
			else 
			{
				m_frame_of_released[button] = m_frame_id;
			}
		}

	};
};