#include "canvas.h"
#include "../../renderer/renderer.h"

namespace Ness
{
	Canvas::Canvas(Renderer* renderer, NodeAPI* parent, const std::string& NewTextureName, const Sizei& size) : Sprite(renderer, parent),
		m_auto_clear(false)
	{
		// create the canvas empty texture and use it
		ManagedResources::ManagedTexturePtr texture = m_renderer->resources().create_blank_texture(NewTextureName, size);
		init_from_texture(texture);
		m_last_clear_time = m_renderer->get_frameid();
	}

	void Canvas::clear()
	{
		m_renderer->clear_texture(get_texture());
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