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
* A camera you can use to "navigate" in scenes
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../exports.h"
#include "../basic_types/point.h"
#include "../basic_types/containers.h"

namespace Ness
{
	/**
	* Cameras are objects you can create and render scenes with, and they will move all entities in the scene to fit camera position.
	* this is your method to "navigate" in a larger node without actually moving entities but rather move the camera instead.
	*/
	class Camera
	{
	public:
		Point position;

		NESSENGINE_API Camera(const Point& Position) : position(Position) {}
		NESSENGINE_API Camera() : position(0, 0) {}

		// some useful operators
		NESSENGINE_API void operator*=(float scalar) {position *= scalar;}
		NESSENGINE_API void operator/=(float scalar) {position /= scalar;}
		NESSENGINE_API void operator-=(float scalar) {position -= scalar;}
		NESSENGINE_API void operator+=(float scalar) {position += scalar;}
		NESSENGINE_API void operator*=(const Point& pos) {position *= pos;}
		NESSENGINE_API void operator/=(const Point& pos) {position /= pos;}
		NESSENGINE_API void operator-=(const Point& pos) {position -= pos;}
		NESSENGINE_API void operator+=(const Point& pos) {position += pos;}
		NESSENGINE_API void operator*=(const Camera& other) {position *= other.position;}
		NESSENGINE_API void operator/=(const Camera& other) {position /= other.position;}
		NESSENGINE_API void operator-=(const Camera& other) {position -= other.position;}
		NESSENGINE_API void operator+=(const Camera& other) {position += other.position;}
	};

	// camera pointer
	NESSENGINE_API typedef SharedPtr<Camera> CameraPtr;

	// null camera
	#define NullCamera CameraPtr()
};