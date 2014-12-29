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
* the API of an object capable of digesting SDL events
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <SDL.h>
#include "../../exports.h"
#include "../../basic_types/events.h"

namespace Ness
{
	namespace Utils
	{
		class EventsHandler
		{
		public:
			// the function that gives the event handler an event to examine.
			// should return true if handled the event, false if irrelevant to it.
			NESSENGINE_API virtual bool inject_event(const Event& event) = 0;
		};
	};
};