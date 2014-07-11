/**
* Main include file for the entire engine! also contain the general init and finish functions
* Author: Ronen Ness
* Since: 07/2014
*/

#pragma once
#include <tchar.h>
#include "exceptions\exceptions.h"
#include "renderer\renderer.h"
#include "primitives\primitives.h"
#include "scene\scene.h"
#include "scene\camera.h"
#include "utils\keyboard.h"

namespace Ness
{
	// MUST BE CALLED BEFORE USING THE ENGINE.
	// imgFlags - image formats to support in addition to BMP.
	void init(int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF);

	// should be called at the end to cleanup stuff.
	void finish();
};