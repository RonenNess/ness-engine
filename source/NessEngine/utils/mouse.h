/**
* mouse wrapper
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../basic_types/point.h"
#include <SDL.h>
#include <unordered_map>
#include "event_handler.h"

namespace Ness
{
	namespace Utils
	{

		// different mouse buttons
		enum EMouseButtons
		{
			MOUSE_LEFT = 0,
			MOUSE_RIGHT = 1,
			MOUSE_MIDDLE = 2,
			_MOUSE_BUTTONS_COUNT,
		};

		// wrap mouse functionality for easy mouse controls
		class Mouse : public EventsHandler
		{
		private:
			Pointi m_last_mouse_pos;
			bool m_down[_MOUSE_BUTTONS_COUNT];

		public:

			// init the mouse wrapper
			NESSENGINE_API Mouse();

			// update the keyboard with incoming event
			NESSENGINE_API virtual bool inject_event(const Event& event);

			// get mouse position
			NESSENGINE_API const Pointi& position();

			// get mouse button state
			inline bool is_down(EMouseButtons button) const {return m_down[button];}

		private:

			void change_button_state(int button, bool isDown);
		};

	};
};