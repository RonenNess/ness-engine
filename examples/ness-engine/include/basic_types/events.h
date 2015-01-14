/* 
  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Ronen Ness
  ronenness@gmail.com

*/

/**
* Alias for the event type
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <SDL.h>
#include "../exports.h"

namespace Ness
{
	// ness-engine events (just SDL events)
	NESSENGINE_API typedef SDL_Event Event;

	// ness-engine keyboard codes (just SDL codes)
	NESSENGINE_API typedef SDL_Keycode Keycode;

	// different mouse buttons
	enum EMouseButtons
	{
		MOUSE_LEFT		= SDL_BUTTON_LEFT,
		MOUSE_RIGHT		= SDL_BUTTON_RIGHT,
		MOUSE_MIDDLE	= SDL_BUTTON_MIDDLE,
		MOUSE_X1		= SDL_BUTTON_X1,
		MOUSE_X2		= SDL_BUTTON_X2,
		_MOUSE_BUTTONS_COUNT,
	};

}