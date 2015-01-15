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
* Define the API of a camera class (camera is something you pass
* to all renderable entities that effects the target rect and transformations
* of the rendering. the most basic usage is to set position but can do other stuff)
* Author: Ronen Ness
* Since: 01/1015
*/

#pragma once
#include "../../exports.h"
#include "../../basic_types/point.h"
#include "../../basic_types/rectangle.h"
#include "../../basic_types/containers.h"
#include "../../renderable/transformations.h"
#include "../../animators/animator_api.h"

namespace Ness
{

	// predeclare Renderer
	class Renderer;

	// predeclare renderable ptr
	class RenderableAPI;

	// hash value that represent camera state
	typedef unsigned long long int TCameraHash;

	/**
	* Cameras are objects you can create and render scenes with, and will effect the renderings of all the entities
	* in the scene. its most basic functionality is to change position but it can also change size and do other
	* interesting transformations.
	*/
	class CameraAPI : public Animators::AnimatorAPI
	{
	protected:
		Renderer*			m_renderer;						// parent renderer pointer

	public:

		NESSENGINE_API CameraAPI(Renderer* renderer) : m_renderer(renderer) {}

		// get a target rect and transformations and change them based on the camera.
		// this is the main function every camera need to implement!
		// entity is a pointer to the entity rendered
		// target_rect is the output rendering target rect the camera can effect (position and size). 
		//		NOTE: it should be already set by the entity based on position and size! camera only modify the target rect not set
		//		it from zero.
		// transformations is the output transformations the camera can effect (color, rotation, etc..). same here, transformations
		//		should be absolute transformations right before the rendering phase.
		NESSENGINE_API virtual void apply_transformations(const RenderableAPI* entity, Rectangle& target_rect, SRenderTransformations& transformations) = 0;

		// this function only set target rect without affecting transformations
		NESSENGINE_API virtual void set_target_rect_only(const RenderableAPI* entity, Rectangle& target_rect, const SRenderTransformations& transformations) = 0;

		// this function only set transformations without affeting the target rect
		NESSENGINE_API virtual void set_transformations_only(const RenderableAPI* entity, const Rectangle& target_rect, SRenderTransformations& transformations) = 0;

		// calculate and return the absolute out position for a given entity, target rectangle, and transformations
		NESSENGINE_API virtual Point get_abs_position(const RenderableAPI* entity, const Rectangle& target_rect, const SRenderTransformations& transformations) = 0;

		// get the position of this camera
		NESSENGINE_API virtual const Point& get_position() const = 0;

		// check if entity should be culled out BEFORE camera transformations apply.
		// note: this should be the minimal culling tests, just the basics. both pre and post checks will be called.
		// entity is the renderable to test
		// target_rect is the target rectangle BEFORE applying the camera on it
		// transformations are absolute transformations BEFORE applying the camera on it
		NESSENGINE_API virtual bool should_cull_pre_transform(const RenderableAPI* entity, const Rectangle& target_rect, const SRenderTransformations& transformations) = 0;

		// check if entity should be culled out AFTER camera transformations apply.
		// note: this is the last culling test before rendering. this is a good place to test if inside screen bounderies etc..
		// entity is the renderable to test
		// target_rect is the target rectangle AFTER the camera was applied on it
		// transformations are absolute transformations AFTER the camera was applied on it
		NESSENGINE_API virtual bool should_cull_post_transform(const RenderableAPI* entity, const Rectangle& target_rect, const SRenderTransformations& transformations) = 0;

		// for ness-engine internal optimizations, this function should return a unique identifier of the state of the camera
		// like a hash on the camera current value(s).
		NESSENGINE_API virtual TCameraHash get_hash() const = 0;
	};

	// camera pointer
	NESSENGINE_API typedef SharedPtr<CameraAPI> CameraApiPtr;
};