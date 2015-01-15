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

#include "entity.h"
#include "../../renderer/renderer.h"
#include "../node_api.h"

namespace Ness
{

	Entity::Entity(Renderer* renderer) : EntityAPI(renderer),
		m_need_transformations_update(true), m_last_render_frame_id(0), m_last_update_frame_id(0), m_highlight(false)
	{
	}

	void Entity::transformations_update() 
	{
		m_last_update_frame_id = m_renderer->get_frameid();
		m_need_transformations_update = true;
	}

	const SRenderTransformations& Entity::get_absolute_transformations_const() const
	{
		return m_absolute_transformations;
	}

	const SRenderTransformations& Entity::get_absolute_transformations()
	{
		// if don't have a parent, return self transformations
		if (!m_parent)
		{
			// recalculate the target rectangle (uses m_absolute_transformations)
			if (m_need_transformations_update)
			{
				m_absolute_transformations = m_transformations;
				calc_target_rect();
				m_need_transformations_update = false;
			}
			return m_transformations;
		}

		// don't need update?
		if (!m_need_transformations_update)
		{
			return m_absolute_transformations;
		}

		// calculate this transformations with parent transformations
		m_absolute_transformations = m_transformations;
		m_absolute_transformations.add_transformations(m_parent->get_absolute_transformations());
		m_need_transformations_update = false;

		// update target rect
		calc_target_rect();

		// return transformations
		return m_absolute_transformations;
	}

	void Entity::calc_target_rect()
	{
		m_target_rect.w = (int)ceil(m_size.x * m_absolute_transformations.scale.x);
		m_target_rect.h = (int)ceil(m_size.y * m_absolute_transformations.scale.y);
		m_target_rect.x = (int)floor((m_absolute_transformations.position.x) - (abs(m_target_rect.w) * m_anchor.x));
		m_target_rect.y = (int)floor((m_absolute_transformations.position.y) - (abs(m_target_rect.h) * m_anchor.y));
	}


	bool Entity::is_really_visible(const CameraApiPtr& camera)
	{
		// first check if even enabled
		if (!m_visible)
		{
			return false;
		}

		// if was rendered during this frame, it's safe enough to assume it is visible
		if ((!m_need_transformations_update) && was_rendered_this_frame())
			return true;

		// check if should cull before transformations
		if (camera->should_cull_pre_transform(this, m_target_rect, m_absolute_transformations))
		{
			return false;
		}

		// set camera position
		SRenderTransformations trans = get_absolute_transformations();
		Rectangle target = m_target_rect;
		camera->apply_transformations(this, target, trans);

		// check if should cull after transformations
		return (camera->should_cull_post_transform(this, target, trans));
	}

	bool Entity::is_really_visible_const(const CameraApiPtr& camera) const
	{
		// first check if even enabled
		if (!m_visible)
			return false;

		// if was rendered during this frame, it's safe enough to assume it is visible
		if (was_rendered_this_frame())
			return true;

		// check culling before applying camera
		if (camera->should_cull_pre_transform(this, m_target_rect, m_absolute_transformations))
			return false;

		// check culling after applying camera
		if (camera->should_cull_post_transform(this, m_target_rect, m_absolute_transformations))
			return false;

		// visible!
		return true;
	}

	bool Entity::was_rendered_this_frame() const
	{ 
		return m_renderer->get_frameid() == m_last_render_frame_id; 
	}

	bool Entity::was_updated_this_frame() const
	{
		return m_renderer->get_frameid() == m_last_update_frame_id;
	}

	void Entity::render(const CameraApiPtr& camera)
	{
		// if invisible skip
		if (!m_visible)
		{
			return;
		}

		// check culling before applying camera
		if (camera->should_cull_pre_transform(this, m_target_rect, m_absolute_transformations))
			return;

		// get absolute transformations and target rect
		SRenderTransformations trans = get_absolute_transformations();
		Rectangle target = m_target_rect;

		// apply camera
		camera->apply_transformations(this, target, trans);

		// check culling after applying camera
		if (camera->should_cull_post_transform(this, target, trans))
			return;

		// set lastly rendered frame
		m_last_render_frame_id = m_renderer->get_frameid();

		// render!
		do_render(target, trans);

		// do highlight effect
		if (m_highlight)
		{
			SRenderTransformations trans_with_add = trans;
			trans_with_add.blend = BLEND_MODE_ADD;
			for (int i = 0; i < m_highlight; ++i)
				do_render(target, trans_with_add);
		}
	}
};