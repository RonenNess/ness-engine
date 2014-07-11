#include "renderable_entity.h"
#include "../renderer/renderer.h"

namespace Ness
{

	RenderableEntity::RenderableEntity(Renderer* renderer, RenderableParent* parent) : Renderable(renderer, parent)
	{
		set_position(Point(0, 0));
		set_anchor(Point::ZERO);
	}

	void RenderableEntity::transformations_update() 
	{
		m_need_update = true;
	}

	const SRenderTransformations& RenderableEntity::get_absolute_transformations()
	{

		// if don't have a parent, return self transformations
		if (!m_parent || m_static)
		{
			if (m_need_update)
			{
				m_absolute_transformations = m_transformations;
				calc_target_rect();
				m_need_update = false;
			}
			return m_transformations;
		}

		// don't need update?
		if (!m_need_update)
			return m_absolute_transformations;

		// calculate this transformations with parent transformations
		m_absolute_transformations = m_transformations;
		m_absolute_transformations.add_transformations(m_parent->get_absolute_transformations());
		m_need_update = false;

		// update target rect
		calc_target_rect();

		// return transformations
		return m_absolute_transformations;
	}

	// calculate target rect
	void RenderableEntity::calc_target_rect()
	{
		m_target_rect.w = (int)(m_size.x * m_absolute_transformations.scale.x);
		m_target_rect.h = (int)(m_size.y * m_absolute_transformations.scale.y);
		m_target_rect.x = (int)((m_absolute_transformations.position.x) - (m_target_rect.w * m_anchor.x));
		m_target_rect.y = (int)((m_absolute_transformations.position.y) - (m_target_rect.h * m_anchor.y));
	}


	// return true if this sprite is in screen
	bool RenderableEntity::is_really_visible(const CameraPtr& camera)
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

		if (target.x >= m_renderer->get_target_size().x || target.y >= m_renderer->get_target_size().y || target.x + target.w <= 0 || target.y + target.h <= 0 )
		{
			return false;
		}

		return true;
	}

	void RenderableEntity::render(const CameraPtr& camera)
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
		if (target.x >= m_renderer->get_target_size().x || target.y >= m_renderer->get_target_size().y || target.x + target.w <= 0 || target.y + target.h <= 0 )
		{
			return;
		}

		// render!
		do_render(target, trans);
	}
};