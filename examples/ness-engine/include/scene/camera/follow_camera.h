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
* The most basic type of a camera, only set position you can use to navigate in scenes
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "basic_camera.h"

namespace Ness
{

	/**
	* A basic camera that can follow entities
	*/
	class FollowCamera : public BasicCamera
	{
	protected:
		RenderablePtr		m_target;						// target this camera may focus on
		Point				m_follow_target_speed;			// speed to foolow target on x and y axis. if 0, will follow immediatly
		Point				m_follow_target_offset;			// offset on x and y axis to follow target
		Point				m_follow_target_max_distance;	// maximum distance the camera can drift away from target
		Rectangle			m_bounderies;					// bounderies for camera position
		bool				m_bounderies_enabled;			// are bounderies enabled?

	public:

		NESSENGINE_API FollowCamera(Renderer* renderer, const Point& Position = Point(0, 0));

		// set / unset target the camera will automatically focus on (center screen on target position)
		NESSENGINE_API inline void set_target(const RenderablePtr& target) {m_target = target;}
		NESSENGINE_API inline void reset_target() {m_target.reset();}

		// set the speed in which the camera follows the target (if provided). if 0, will be immediate follow ("teleport" to position)
		NESSENGINE_API inline void set_target_follow_speed(const Point& speed) {m_follow_target_speed = speed;}

		// set the target offset, i.e. the camera will follow target position + given offset
		NESSENGINE_API inline void set_target_follow_offset(const Point& offset) {m_follow_target_offset = offset;}

		// set bounderies for this camera
		// to disable bounderies check set rectangle of 0, 0, 0, 0
		NESSENGINE_API inline void set_bounderies(const Rectangle& bounderies) {m_bounderies = bounderies; m_bounderies_enabled = ((bounderies.x | bounderies.y | bounderies.w | bounderies.h) != 0);}

		// set the maximum distance the camera can drift away from the following target. 0 is distance unlimited.
		// note: this is relevant only if you got following speed and target can drift away from the camera if moving too fast.
		NESSENGINE_API inline void set_target_follow_max_distance(const Point& distance) {m_follow_target_max_distance = distance;}

		// animate the camera
		NESSENGINE_API virtual void do_animation(Renderer* renderer);
	};

	// camera pointer
	NESSENGINE_API typedef SharedPtr<FollowCamera> FollowCameraPtr;
};