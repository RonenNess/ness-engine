/**
* A camera you can use to "navigate" in scenes
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../primitives/point.h"
#include <memory>

namespace Ness
{
	class Camera
	{
	public:
		Point position;

		Camera() : position(0, 0) {}
	};

	// camera pointer
	typedef std::shared_ptr<Camera> CameraPtr;

	// null camera
	#define NullCamera CameraPtr()
};