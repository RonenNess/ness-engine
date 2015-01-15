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

#include "viewport.h"
#include "../renderable/renderable_api.h"
#include "../renderer/renderer.h"

namespace Ness
{
	Viewport::Viewport(Renderer* renderer, const Sizei& source_size) : m_renderer(renderer)
	{
		reset(source_size);
	}

	void Viewport::reset(const Sizei& source_size)
	{
		if (m_canvas) m_canvas.reset();
		static unsigned int unique_id = 0;
		m_canvas = ness_make_ptr<Canvas>(m_renderer, "viewport__" + ness_int_to_string(unique_id++));
	}

	void Viewport::render()
	{
		m_canvas->render(m_renderer->get_null_camera());
	}

	void Viewport::set_dest_position(const Point& position)
	{
		m_canvas->set_position(position);
	}

	void Viewport::set_dest_size(const Size& size)
	{
		m_canvas->set_size(size);
	}

	void Viewport::set_mask(const String& textureFile) 
	{
		m_canvas->set_mask(textureFile);
	}

	void Viewport::remove_mask()
	{
		m_canvas->remove_mask();
	}

	void Viewport::set_dest_region(const Rectangle& region)
	{
		set_dest_position(Pointi(region.x, region.y) - (Size((float)region.w, (float)region.h) * m_canvas->get_anchor()));
		set_dest_size(Sizei(region.w, region.h));
	}

	void Viewport::set_scale(const Point& scale)
	{
		m_canvas->set_scale(scale);
	}

	void Viewport::set_anchor(const Point& anchor)
	{
		m_canvas->set_anchor(anchor);
	}

	void Viewport::flip_x()
	{
		m_canvas->set_scale(m_canvas->get_scale() * Point(-1.0f, 1.0f));
	}

	void Viewport::flip_y()
	{
		m_canvas->set_scale(m_canvas->get_scale() * Point(1.0f, -1.0f));
	}

	void Viewport::set_color(const Color& color)
	{
		m_canvas->set_color(color);
	}

	void Viewport::set_rotation(float rotation)
	{
		m_canvas->set_rotation(rotation);
	}
}