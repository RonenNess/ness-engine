/**
* A camera you can use to "navigate" in scenes
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../exports.h"
#include "../primitives/point.h"
#include <memory>

namespace Ness
{
	class Camera
	{
	public:
		Point position;

		NESSENGINE_API Camera() : position(0, 0) {}
	};

	// camera pointer
	NESSENGINE_API typedef std::shared_ptr<Camera> CameraPtr;

	// null camera
	#define NullCamera CameraPtr()
};