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
* A special camera that does nothing and check very basic culling.
* For internal usage.
* Author: Ronen Ness
* Since: 01/1015
*/

#pragma once
#include "camera_api.h"

namespace Ness
{

	/**
	* A special camera that does nothing and check very basic culling.
	* this camera is used internally when no camera is provided for the rendering.
	*/
	class NullCamera : public CameraAPI
	{
	private:

	public:

		NESSENGINE_API NullCamera(Renderer* renderer);

		// do culling checks
		NESSENGINE_API virtual bool should_cull_pre_transform(const RenderableAPI* entity, const Rectangle& target_rect, const SRenderTransformations& transformations);
		NESSENGINE_API virtual bool should_cull_post_transform(const RenderableAPI* entity, const Rectangle& target_rect, const SRenderTransformations& transformations);
		
		// apply this camera! (do nothing)
		NESSENGINE_API virtual void apply_transformations(const RenderableAPI* entity, Rectangle& target_rect, SRenderTransformations& transformations) {}
		NESSENGINE_API virtual void set_target_rect_only(const RenderableAPI* entity, Rectangle& target_rect, const SRenderTransformations& transformations) {}
		NESSENGINE_API virtual void set_transformations_only(const RenderableAPI* entity, const Rectangle& target_rect, SRenderTransformations& transformations) {}

		// calculate and return the absolute out position for a given entity, target rectangle, and transformations
		NESSENGINE_API virtual Point get_abs_position(const RenderableAPI* entity, const Rectangle& target_rect, const SRenderTransformations& transformations);


		// animate the camera
		NESSENGINE_API virtual void do_animation(Renderer* renderer) {}

		// return position 0, 0
		NESSENGINE_API virtual const Point& get_position() const;

		// get camera hash
		NESSENGINE_API virtual TCameraHash get_hash() const {return 0;}
	};

	// camera pointer
	NESSENGINE_API typedef SharedPtr<NullCamera> NullCameraPtr;

};