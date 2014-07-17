#include "entity.h"
#include "../../renderer/renderer.h"
#include "../node_api.h"

namespace Ness
{

	Entity::Entity(Renderer* renderer) : EntityAPI(renderer), m_static(false), m_need_transformations_update(true)
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
		if (!m_parent || m_static)
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
			return m_absolute_transformations;

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
		m_target_rect.w = (int)(m_size.x * m_absolute_transformations.scale.x);
		m_target_rect.h = (int)(m_size.y * m_absolute_transformations.scale.y);
		m_target_rect.x = (int)((m_absolute_transformations.position.x) - (abs(m_target_rect.w) * m_anchor.x));
		m_target_rect.y = (int)((m_absolute_transformations.position.y) - (abs(m_target_rect.h) * m_anchor.y));
	}


	bool Entity::is_really_visible(const CameraPtr& camera)
	{
		// get absolute transformations
		const SRenderTransformations& trans = get_absolute_transformations();

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

		return is_in_screen(target);
	}

	bool Entity::is_really_visible_const(const CameraPtr& camera) const
	{
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

		return is_in_screen(target);
	}

	bool Entity::is_in_screen(const Rectangle& target) const
	{
		if (target.x >= m_renderer->get_target_size().x || target.y >= m_renderer->get_target_size().y || target.x + abs(target.w) <= 0 || target.y + abs(target.h) <= 0 )
		{
			return false;
		}
		return true;
	}

	void Entity::render(const CameraPtr& camera)
	{
		// if invisible skip
		if (!m_visible)
			return;

		// get absolute transformations
		const SRenderTransformations& trans = get_absolute_transformations();

		// invisible?
		if (trans.color.a <= 0)
			return;

		// set camera position
		Rectangle target = m_target_rect;
		if (!m_static && camera)
		{
			target.x -= (int)camera->position.x;
			target.y -= (int)camera->position.y;
		}

		// check if in screen. NOTE: this checked inside the renderer as well, but having this check here boost up speed with tons of entities (> 10,000...)
		if (!is_in_screen(target))
			return;

		// render!
		do_render(target, trans);
	}
};