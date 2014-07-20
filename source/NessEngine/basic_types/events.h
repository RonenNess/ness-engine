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
	NESSENGINE_API typedef SDL_Event Event;
	NESSENGINE_API typedef SDL_Keycode Keycode;
}