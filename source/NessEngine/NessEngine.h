/**
* Main include file for the entire engine! also contain the general init and finish functions
* Author: Ronen Ness
* Since: 07/2014
*/

#pragma once
#include "exports.h"
#include <SDL.h>
#include <tchar.h>

// include renderer and basic stuff
#include "renderer\renderer.h"
#include "exceptions\exceptions.h"
#include "primitives\primitives.h"

// include scene and camera
#include "scene\scene.h"
#include "scene\camera.h"

// include utils
#include "utils\keyboard.h"
#include "utils\mouse.h"
#include "utils\events_poller.h"

// include all renderables
#include "renderable/node.h"
#include "renderable/znode.h"
#include "renderable/sprite.h"
#include "renderable/tile_map.h"
#include "renderable/canvas.h"
#include "renderable/shapes.h"

namespace Ness
{
	// current ness engine version
	#define NESS_VERSION 1.0

	// MUST BE CALLED BEFORE USING THE ENGINE.
	// imgFlags - image formats to support in addition to BMP.
	NESSENGINE_API void init(int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF);

	// get next system event. return true if got more events to read, false otherwise
	// note: this is just a wrapper to SDL events mechanism, and exist to allow users to fetch events without having to import the SDL libs
	NESSENGINE_API typedef SDL_Event Event;
	NESSENGINE_API bool get_poll_event(Event& event);

	// should be called at the end to cleanup stuff.
	NESSENGINE_API void finish();
};