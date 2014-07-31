#include "sprite.h"
#include "../../renderer/renderer.h"

namespace Ness
{
	// defaults
	SSpriteDefaults Sprite::Defaults;

	Sprite::Sprite(Renderer* renderer) : Entity(renderer)
	{
		set_defaults();
	}

	Sprite::Sprite(Renderer* renderer, ManagedResources::ManagedTexturePtr texture) : Entity(renderer)
	{
		set_defaults();
		init_from_texture(texture);
	}

	void Sprite::change_texture(const String& NewTextureFile)
	{
		m_texture = m_renderer->resources().get_texture(NewTextureFile);
	}


	Sprite::Sprite(Renderer* renderer, const String& TextureFile) : Entity(renderer)
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

	void Sprite::set_source_from_sprite_sheet(const Pointi& step, const Sizei stepsCount, bool setSize)
	{
		int x_step = (int)((1.0f / stepsCount.x) * m_texture->get_size().x);
		int y_step = (int)((1.0f / stepsCount.y) * m_texture->get_size().y);
		m_source_rect.x = x_step * step.x;
		m_source_rect.y = y_step * step.y;
		m_source_rect.w = x_step;
		m_source_rect.h = y_step;
		if (setSize)
		{
			set_size(m_texture->get_size() / stepsCount);
		}
	}

	void Sprite::do_render(const Rectangle& target, const SRenderTransformations& transformations)
	{
		m_renderer->blit(m_texture, &m_source_rect, target, transformations.blend, transformations.color, transformations.rotation, m_anchor);
	}
};