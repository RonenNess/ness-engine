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
		change_texture(texture, true);
	}

	void Sprite::change_texture(ManagedResources::ManagedTexturePtr NewTexture, bool resetSizeAndSource)
	{
		m_texture = NewTexture;
		if (resetSizeAndSource)
		{
			set_size(Size((float)m_texture->get_size().x, (float)m_texture->get_size().y));
			reset_source_rect();
		}
	}

	void Sprite::change_texture(const String& NewTextureFile, bool resetSizeAndSource)
	{
		change_texture(m_renderer->resources().get_texture(NewTextureFile), resetSizeAndSource);
	}


	Sprite::Sprite(Renderer* renderer, const String& TextureFile) : Entity(renderer)
	{
		set_defaults();
		if (TextureFile.length() > 0)
		{
			ManagedResources::ManagedTexturePtr texture = m_renderer->resources().get_texture(TextureFile);
			change_texture(texture, true);
		}
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