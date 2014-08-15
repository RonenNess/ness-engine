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

#include "canvas.h"
#include "../../renderer/renderer.h"

namespace Ness
{
	Canvas::Canvas(Renderer* renderer, const String& NewTextureName, const Sizei& size) : Sprite(renderer),
		m_auto_clear(false), m_clean_color(0, 0, 0, 0)
	{
		// create the canvas empty texture and use it
		ManagedResources::ManagedTexturePtr texture = m_renderer->resources().create_blank_texture(NewTextureName, size);
		init_from_texture(texture);
		m_last_clear_time = m_renderer->get_frameid();
	}

	void Canvas::clear()
	{
		if (m_clean_color.a == 0)
		{
			m_renderer->clear_texture(get_texture());
		}
		else
		{
			m_renderer->fill_texture(get_texture(), m_clean_color);
		}
	}

	void Canvas::render(const CameraPtr& camera)
	{
		// call the basic sprite render function
		Sprite::render(camera);

		// if autoclear enabled, clear
		if (m_auto_clear)
		{
			if (m_last_clear_time != m_renderer->get_frameid())
			{
				clear();
				m_last_clear_time = m_renderer->get_frameid();
			}
		}
	}
};