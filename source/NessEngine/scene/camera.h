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
#include "../animators/animator_api.h"

namespace Ness
{
	// predeclare renderable ptr
	class RenderableAPI;
	NESSENGINE_API typedef SharedPtr<RenderableAPI> RenderablePtr;

	// predeclare Renderer
	class Renderer;

	/**
	* Cameras are objects you can create and render scenes with, and they will move all entities in the scene to fit camera position.
	* this is your method to "navigate" in a larger node without actually moving entities but rather move the camera instead.
	*/
	class Camera : public Animators::AnimatorAPI
	{
	private:
		Renderer*			m_renderer;						// parent renderer pointer
		RenderablePtr		m_target;						// target this camera may focus on
		Point				m_follow_target_speed;			// speed to foolow target on x and y axis. if 0, will follow immediatly
		Point				m_follow_target_offset;			// offset on x and y axis to follow target
		Point				m_follow_target_max_distance;	// maximum distance the camera can drift away from target

	public:
		Point position;

		NESSENGINE_API Camera(Renderer* renderer, const Point& Position = Point(0, 0));
		NESSENGINE_API ~Camera();

		// set / unset target the camera will automatically focus on (center screen on target position)
		NESSENGINE_API inline void set_target(const RenderablePtr& target) {m_target = target;}
		NESSENGINE_API inline void reset_target() {m_target.reset();}

		// set the speed in which the camera follows the target (if provided). if 0, will be immediate follow ("teleport" to position)
		NESSENGINE_API inline void set_target_follow_speed(const Point& speed) {m_follow_target_speed = speed;}

		// set the target offset, i.e. the camera will follow target position + given offset
		NESSENGINE_API inline void set_target_follow_offset(const Point& offset) {m_follow_target_offset = offset;}

		// set the maximum distance the camera can drift away from the following target. 0 is distance unlimited.
		// note: this is relevant only if you got following speed and target can drift away from the camera if moving too fast.
		NESSENGINE_API inline void set_target_follow_max_distance(const Point& distance) {m_follow_target_max_distance = distance;}

		// animate the camera
		NESSENGINE_API virtual void do_animation(Renderer* renderer);

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