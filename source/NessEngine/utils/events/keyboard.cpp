#include "keyboard.h"

namespace Ness
{
	namespace Utils
	{

		bool Keyboard::inject_event(const Event& event)
		{
			switch (event.type)
			{
				case SDL_KEYDOWN:
					m_key_codes[event.key.keysym.sym] = true;
					m_frame_of_down[event.key.keysym.sym] = m_frame_id;
					return true;

				case SDL_KEYUP:
					m_key_codes[event.key.keysym.sym] = false;
					m_frame_of_released[event.key.keysym.sym] = m_frame_id;
					return true;
			}
			return false;
		}

		bool Keyboard::was_clicked(Keycode key, float threshold_in_seconds)
		{
			return ((m_frame_of_released[key] == m_frame_id) && m_frame_of_down[key] + (threshold_in_seconds * 1000.0f) >= m_frame_id);
		}

		void Keyboard::start_frame() 
		{
			m_frame_id = SDL_GetTicks();
		}
	};
};