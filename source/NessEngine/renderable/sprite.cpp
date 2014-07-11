#include "sprite.h"
#include "../renderable/renderable.h"
#include "../renderer/renderer.h"

namespace Ness
{
	// defaults
	SSpriteDefaults Sprite::Defaults;

	Sprite::Sprite(Renderer* renderer, RenderableParent* parent) : Renderable(renderer, parent)
	{
		set_defaults();
	}

	Sprite::Sprite(Renderer* renderer, RenderableParent* parent, ManagedResources::ManagedTexturePtr texture) : Renderable(renderer, parent)
	{
		set_defaults();
		init_from_texture(texture);
	}

	Sprite::Sprite(Renderer* renderer, RenderableParent* parent, const std::string& TextureFile) : Renderable(renderer, parent)
	{
		set_defaults();
		ManagedResources::ManagedTexturePtr texture = m_renderer->resources().get_texture(TextureFile);
		init_from_texture(texture);
	}

	void Sprite::set_defaults()
	{
		m_static = Sprite::Defaults.is_static;
		if (Sprite::Defaults.alpha_channels && m_transformations.blend == BLEND_MODE_NONE)
		{
			m_transformations.blend = BLEND_MODE_BLEND;
		}
		m_anchor = Sprite::Defaults.anchor;
	}

	void Sprite::transformations_update() 
	{
		m_need_update = true;
	}

	const SRenderTransformations& Sprite::get_absolute_transformations()
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
	void Sprite::calc_target_rect()
	{
		m_target_rect.w = (int)(m_size.x * m_absolute_transformations.scale.x);
		m_target_rect.h = (int)(m_size.y * m_absolute_transformations.scale.y);
		m_target_rect.x = (int)((m_absolute_transformations.position.x) - (m_target_rect.w * m_anchor.x));
		m_target_rect.y = (int)((m_absolute_transformations.position.y) - (m_target_rect.h * m_anchor.y));
	}

	void Sprite::init_from_texture(ManagedResources::ManagedTexturePtr texture)
	{
		set_size(Size((float)texture->get_size().x, (float)texture->get_size().y));
		set_position(Point(0, 0));
		change_texture(texture);
		reset_source_rect();
	}

	void Sprite::reset_source_rect()
	{
		m_source_rect.x = 0;
		m_source_rect.y = 0;
		m_source_rect.w = (int)m_texture->get_size().x;
		m_source_rect.h = (int)m_texture->get_size().y;
	}

	void Sprite::set_source_rect(const Rectangle& srcRect)
	{
		m_source_rect = srcRect;
	}

	void Sprite::set_source_from_sprite_sheet(const Pointi& step, const Sizei stepsCount)
	{
		int x_step = (int)((1.0f / stepsCount.x) * m_texture->get_size().x);
		int y_step = (int)((1.0f / stepsCount.y) * m_texture->get_size().y);
		m_source_rect.x = x_step * step.x;
		m_source_rect.y = y_step * step.y;
		m_source_rect.w = x_step;
		m_source_rect.h = y_step;
	}

	// return true if this sprite is in screen
	bool Sprite::is_really_visible(const CameraPtr& camera)
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

	void Sprite::render(const CameraPtr& camera)
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
		m_renderer->blit(m_texture, m_source_rect, target, trans.blend, trans.color, trans.rotation, m_anchor);
	}
};