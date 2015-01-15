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
#include "camera_api.h"

namespace Ness
{

	/**
	* A basic camera that affect rendering position only
	*/
	class BasicCamera : public CameraAPI
	{
	public:
		// the position of this camera
		Point position;

		// special position that will force-effect even static entities.
		// this is for internal usage and may be removed later. don't count on using it.
		Point __statics_position;

		NESSENGINE_API BasicCamera(Renderer* renderer, const Point& Position = Point(0, 0));
		NESSENGINE_API ~BasicCamera();

		// do culling checks
		NESSENGINE_API virtual bool should_cull_pre_transform(const RenderableAPI* entity, const Rectangle& target_rect, const SRenderTransformations& transformations);
		NESSENGINE_API virtual bool should_cull_post_transform(const RenderableAPI* entity, const Rectangle& target_rect, const SRenderTransformations& transformations);

		// calculate and return the absolute out position for a given entity, target rectangle, and transformations
		NESSENGINE_API virtual Point get_abs_position(const RenderableAPI* entity, const Rectangle& target_rect, const SRenderTransformations& transformations);

		// apply this camera!
		NESSENGINE_API virtual void apply_transformations(const RenderableAPI* entity, Rectangle& target_rect, SRenderTransformations& transformations);
		NESSENGINE_API virtual void set_target_rect_only(const RenderableAPI* entity, Rectangle& target_rect, const SRenderTransformations& transformations);
		NESSENGINE_API virtual void set_transformations_only(const RenderableAPI* entity, const Rectangle& target_rect, SRenderTransformations& transformations);

		// animate the camera
		NESSENGINE_API virtual void do_animation(Renderer* renderer);

		// return camera hash value (a value that represent the unique state of the camera
		NESSENGINE_API virtual TCameraHash get_hash() const;

		// return camera position
		NESSENGINE_API virtual const Point& get_position() const {return position;}

		// some useful operators
		NESSENGINE_API void operator*=(float scalar) {position *= scalar;}
		NESSENGINE_API void operator/=(float scalar) {position /= scalar;}
		NESSENGINE_API void operator-=(float scalar) {position -= scalar;}
		NESSENGINE_API void operator+=(float scalar) {position += scalar;}
		NESSENGINE_API void operator*=(const Point& pos) {position *= pos;}
		NESSENGINE_API void operator/=(const Point& pos) {position /= pos;}
		NESSENGINE_API void operator-=(const Point& pos) {position -= pos;}
		NESSENGINE_API void operator+=(const Point& pos) {position += pos;}
		NESSENGINE_API void operator*=(const BasicCamera& other) {position *= other.position;}
		NESSENGINE_API void operator/=(const BasicCamera& other) {position /= other.position;}
		NESSENGINE_API void operator-=(const BasicCamera& other) {position -= other.position;}
		NESSENGINE_API void operator+=(const BasicCamera& other) {position += other.position;}
	};

	// camera pointer
	NESSENGINE_API typedef SharedPtr<BasicCamera> CameraPtr;
};