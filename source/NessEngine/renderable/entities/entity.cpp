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

	Entity::Entity(Renderer* renderer) : EntityAPI(renderer), m_static(false), m_need_transformations_update(true), m_last_render_frame_id(0)
	{
		set_position(Point(0, 0));
		set_anchor(Point::ZERO);
	}

	void Entity::transformations_update() 
	{
		m_need_transformations_update = true;
	}

	const SRenderTransformations& Entity::get_absolute_transformations_const() const
	{
		if (!m_parent || m_static)
			return m_transformations;
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


	bool Entity::is_really_visible(const CameraPtr& camera)
	{
		// first check if even enabled
		if (!m_visible)
		{
			return false;
		}

		// get absolute transformations
		const SRenderTransformations& trans = get_absolute_transformations();

		// invisible?
		if (trans.color.a <= 0)
		{
			return false;
		}

		// set camera position
		Rectangle target = m_target_rect;
		if (!m_static && camera)
		{
			target.x -= (int)camera->position.x;
			target.y -= (int)camera->position.y;
		} 

		return is_in_screen(target, trans.rotation);
	}

	bool Entity::is_really_visible_const(const CameraPtr& camera) const
	{
		// first check if even enabled
		if (!m_visible)
			return false;

		// get absolute transformations
		const SRenderTransformations& trans = get_absolute_transformations_const();

		// invisible?
		if (trans.color.a <= 0)
			return false;

		// set camera position
		Rectangle target = m_target_rect;
		if (!m_static && camera)
		{
			target.x -= (int)camera->position.x;
			target.y -= (int)camera->position.y;
		} 

		return is_in_screen(target, trans.rotation);
	}

	bool Entity::is_in_screen(const Rectangle& target, float rotation) const
	{
		// if no rotation make simple rect-in-screen check
		if (rotation == 0.0f)
		{
			if (target.x >= m_renderer->get_target_size().x || target.y >= m_renderer->get_target_size().y || target.x + abs(target.w) <= 0 || target.y + abs(target.h) <= 0 )
			{
				return false;
			}
		}
		// if got rotation fix target rect
		else
		{
			float size = (abs(target.h) > abs(target.w)) ? (float)abs(target.h) : (float)abs(target.w);
			size *= 1.5f;
			if (target.x - size >= m_renderer->get_target_size().x || target.y - size >= m_renderer->get_target_size().y || target.x + size <= 0 || target.y + size <= 0 )
			{
				return false;
			}
		}

		return true;
	}

	bool Entity::was_rendered_this_frame() const
	{ 
		return m_renderer->get_frameid() == m_last_render_frame_id; 
	}

	void Entity::render(const CameraPtr& camera)
	{
		// if invisible skip
		if (!m_visible)
		{
			return;
		}

		// get absolute transformations
		const SRenderTransformations& trans = get_absolute_transformations();

		// invisible?
		if (trans.color.a <= 0)
		{
			return;
		}

		// set camera position
		Rectangle target = m_target_rect;
		if (!m_static && camera)
		{
			target.x -= (int)camera->position.x;
			target.y -= (int)camera->position.y;
		}

		// check if in screen.
		if (!is_in_screen(target, trans.rotation))
		{
			return;
		}

		// lastly visible
		m_last_render_frame_id = m_renderer->get_frameid();

		// render!
		do_render(target, trans);
	}
};