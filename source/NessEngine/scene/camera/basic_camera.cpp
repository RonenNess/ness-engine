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

#include "basic_camera.h"
#include "../../renderable/renderable_api.h"
#include "../../renderer/renderer.h"

namespace Ness
{

	BasicCamera::BasicCamera(Renderer* renderer, const Point& Position) 
		: CameraAPI(renderer), position(Position)
	{
	}

	BasicCamera::~BasicCamera()
	{
	}

	bool BasicCamera::should_cull_pre_transform(const RenderableAPI* entity, const Rectangle& target_rect, const SRenderTransformations& transformations)
	{
		// is this entity not visible?
		if (transformations.color.a <= 0)
			return true;

		return false;
	}

	bool BasicCamera::should_cull_post_transform(const RenderableAPI* entity, const Rectangle& target_rect, const SRenderTransformations& transformations)
	{
		// is this entity not visible with absolute transformations?
		if (transformations.color.a <= 0)
			return true;

		// if no rotation make simple rect-in-screen check
		if (transformations.rotation == 0.0f)
		{
			if (target_rect.x >= m_renderer->get_target_size().x || target_rect.y >= m_renderer->get_target_size().y || 
				target_rect.x + abs(target_rect.w) <= 0 || target_rect.y + abs(target_rect.h) <= 0 )
			{
				return true;
			}
		}
		// if got rotation add size factor to handle the rotation
		else
		{
			float size = (abs(target_rect.h) > abs(target_rect.w)) ? (float)abs(target_rect.h) : (float)abs(target_rect.w);
			size *= 1.5f;
			if (target_rect.x - size >= m_renderer->get_target_size().x || target_rect.y - size >= m_renderer->get_target_size().y || 
				target_rect.x + size <= 0 || target_rect.y + size <= 0 )
			{
				return true;
			}
		}

		// no need for culling :)
		return false;
	}

	Point BasicCamera::get_abs_position(const RenderableAPI* entity, const Rectangle& target_rect, const SRenderTransformations& transformations)
	{
		Rectangle rect = target_rect;
		set_target_rect_only(entity, rect, transformations);
		return Point((float)rect.x, (float)rect.y);
	}

	void BasicCamera::apply_transformations(const RenderableAPI* entity, Rectangle& target_rect, SRenderTransformations& transformations)
	{
		set_transformations_only(entity, target_rect, transformations);
		set_target_rect_only(entity, target_rect, transformations);
	}

	void BasicCamera::set_transformations_only(const RenderableAPI* entity, const Rectangle& target_rect, SRenderTransformations& transformations)
	{
	}

	void BasicCamera::set_target_rect_only(const RenderableAPI* entity, Rectangle& target_rect, const SRenderTransformations& transformations)
	{
		// affect static entities
		if (entity->is_static())
		{
			target_rect.x -= (int)floor(__statics_position.x);
			target_rect.y -= (int)floor(__statics_position.y);
			return;
		}

		// set rectangle for all regular entities
		target_rect.x -= (int)floor(position.x);
		target_rect.y -= (int)floor(position.y);
	}

	void BasicCamera::do_animation(Renderer* renderer)
	{
	}

	TCameraHash BasicCamera::get_hash() const
	{
		return ((TCameraHash)(position.x) & 0xffffffff) | ((TCameraHash)(position.y) & 0xffffffff) << 32;
	}
};