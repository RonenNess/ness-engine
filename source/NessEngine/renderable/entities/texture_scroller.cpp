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
* Texture Scrollers are objects that render tiled texture that can scroll based on camera
* for example, this is useful for animated backgrounds
* Author: Ronen Ness
* Since: 10/1014
*/

#pragma once
#include "texture_scroller.h"
#include "../../renderer/renderer.h"

namespace Ness
{
	
	TextureScroller::TextureScroller(Renderer* renderer, const String& TextureFile)
		: Entity(renderer), m_camera_speed_factor(1.0f, 1.0f), m_base_offset(0, 0)
	{
		set_defaults();
		ManagedResources::ManagedTexturePtr texture = m_renderer->resources().get_texture(TextureFile);
		init_from_texture(texture);
	}


	TextureScroller::TextureScroller(Renderer* renderer, ManagedResources::ManagedTexturePtr texture)
		: Entity(renderer), m_camera_speed_factor(1.0f, 1.0f), m_base_offset(0, 0)
	{
		set_defaults();
		init_from_texture(texture);
	}

	void TextureScroller::change_texture(const String& NewTextureFile)
	{
		m_texture = m_renderer->resources().get_texture(NewTextureFile);
	}

	void TextureScroller::init_from_texture(ManagedResources::ManagedTexturePtr texture)
	{
		set_size(Size((float)texture->get_size().x, (float)texture->get_size().y));
		set_position(Point(0, 0));
		change_texture(texture);
	}

	void TextureScroller::set_defaults()
	{
		m_static = false;
		m_transformations.blend = BLEND_MODE_NONE;
		m_anchor = Point(0, 0);
	}

	void TextureScroller::render(const CameraPtr& camera)
	{
		// first set offset based on camera
		if (camera)
		{
			m_camera_offset = camera->position * m_camera_speed_factor;
		}
		else
		{
			m_camera_offset.x = 0;
			m_camera_offset.y = 0;
		}

		// calc uv from base offset and camera offset
		m_uv = m_base_offset + m_camera_offset;
		m_uv %= m_texture->get_size();
		if (m_uv.x < 0) m_uv.x = m_texture->get_size().x + m_uv.x;
		if (m_uv.y < 0) m_uv.y = m_texture->get_size().y + m_uv.y;
		m_uv /= m_texture->get_size();

		// now call the parent render func
		Entity::render(camera);
	}

	void TextureScroller::do_render(const Rectangle& target, const SRenderTransformations& transformations)
	{
		// set base source rect
		Rectangle src;
		Point invertUV = Point::ONE - m_uv;
		const Pointi& textureSize = m_texture->get_size();
		
		// calc target rect stuff
		int first_half_tar_width = (int)((float)target.w * invertUV.x);
		int first_half_tar_height = (int)((float)target.h * invertUV.y);

		// calc source rect stuff
		int first_half_src_x = (int)(m_uv.x * (float)textureSize.x);
		int first_half_src_y = (int)(m_uv.y * (float)textureSize.y);
		int first_half_src_width = (int)(invertUV.x * (float)textureSize.x);
		int first_half_src_height = (int)(invertUV.y * (float)textureSize.y);

		// render top left corner
		{
			Rectangle src(first_half_src_x, first_half_src_y, first_half_src_width, first_half_src_height);
			if (src.w != 0 && src.h != 0)
			{
				Rectangle tar(target.x, target.y, first_half_tar_width, first_half_tar_height);
				m_renderer->blit(m_texture, &src, tar, transformations.blend, transformations.color, transformations.rotation, m_anchor);
			}
		}
		// render top right corner
		{
			Rectangle src(0, first_half_src_y, first_half_src_x, first_half_src_height);
			if (src.w != 0 && src.h != 0)
			{
				Rectangle tar(target.x + first_half_tar_width, target.y, target.w - first_half_tar_width, first_half_tar_height);
				m_renderer->blit(m_texture, &src, tar, transformations.blend, transformations.color, transformations.rotation, m_anchor);
			}
		}
		// render bottom left corner
		{
			Rectangle src(first_half_src_x, 0, first_half_src_width, first_half_src_y);
			if (src.w != 0 && src.h != 0)
			{
				Rectangle tar(target.x, target.y + first_half_tar_height, first_half_tar_width, target.h - first_half_tar_height);
				m_renderer->blit(m_texture, &src, tar, transformations.blend, transformations.color, transformations.rotation, m_anchor);
			}
		}
		// render bottom right corner
		{
			Rectangle src(0, 0, first_half_src_x, first_half_src_y);
			if (src.w != 0 && src.h != 0)
			{
				Rectangle tar(target.x + first_half_tar_width, target.y + first_half_tar_height, target.w - first_half_tar_width, target.h - first_half_tar_height);
				m_renderer->blit(m_texture, &src, tar, transformations.blend, transformations.color, transformations.rotation, m_anchor);
			}
		}
	}
};