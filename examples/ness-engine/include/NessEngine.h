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
#include "scene\scene.h"
#include "scene\camera.h"

// include renderer and basic stuff
#include "renderer\renderer.h"
#include "exceptions\exceptions.h"
#include "basic_types\all_basic_types.h"

// include utils
#include "utils\keyboard.h"
#include "utils\mouse.h"
#include "utils\application_events.h"
#include "utils\events_poller.h"

// include all renderables
#include "renderable/nodes/all_nodes.h"
#include "renderable/entities/all_entities.h"

// include all animators
#include "animators/all_animators.h"

namespace Ness
{
	// current ness engine version
	#define NESS_VERSION 1.0

	// MUST BE CALLED BEFORE USING THE ENGINE.
	// imgFlags - image formats to support in addition to BMP.
	NESSENGINE_API void init(int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF);

	// get next system event. return true if got more events to read, false otherwise
	// note: this is just a wrapper to SDL events mechanism, and exist to allow users to fetch events without having to import the SDL libs
	NESSENGINE_API bool poll_event(Event& event);

	// don't call this yourself, its called automatically when application ends
	void __finish();
};