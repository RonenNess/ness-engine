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
* Main include file for the entire engine! also contain the general init and finish functions
* Author: Ronen Ness
* Since: 07/2014
*/

#pragma once
#include "exports.h"
#include <SDL.h>
#include <SDL_ttf.h>
#include <tchar.h>

// include scene and camera
#include "scene/scene.h"
#include "scene/camera.h"

// include renderer and basic stuff
#include "renderer/renderer.h"
#include "exceptions/exceptions.h"
#include "basic_types/all_basic_types.h"

// include utils
#include "utils/events/keyboard.h"
#include "utils/events/mouse.h"
#include "utils/events/application_events.h"
#include "utils/events/events_poller.h"
#include "utils/rendering/logo_show.h"

// include all renderables
#include "renderable/renderable_api.h"
#include "renderable/nodes/all_nodes.h"
#include "renderable/entities/all_entities.h"

// include all animators
#include "animators/all_animators.h"

namespace Ness
{
	// current ness-engine version
	#define NESS_VERSION 1.4
	#define NESS_SUB_VERSION 9
	#define NESS_VERSION_STR "1.4.9"

	// MUST BE CALLED BEFORE USING THE ENGINE.
	// imgFlags - image formats to support in addition to BMP.
	NESSENGINE_API void init(int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF);

	// get next system event. return true if got more events to read, false otherwise
	// note: this is just a wrapper to SDL events mechanism, and exist to allow users to fetch events without having to import the SDL libs
	NESSENGINE_API bool poll_event(Event& event);

	// return current tick count
	NESSENGINE_API unsigned int get_ticks();

	// don't call this yourself, its called automatically when application ends
	void __finish();
};