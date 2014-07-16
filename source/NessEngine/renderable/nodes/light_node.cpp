#include "light_node.h"
#include "../../renderer/renderer.h"
#include "../entities/canvas.h"

namespace Ness
{
	LightNode::LightNode(Renderer* renderer) : BaseNode(renderer), m_need_update(true), m_always_update(false)
	{
		// create the canvas.
		// we will render everything on the canvas as additive, and then render the canvas itself with mod blend
		// note: canvas clear color will represent the ambient color, i.e. the color of light when there's no lighting.
		static unsigned int uniqueId = 0;
		m_canvas = NESS_MAKE_PTR<Canvas>(this->m_renderer, std::string("light_node_") + std::to_string((long long)uniqueId));
		uniqueId++;
		set_blend_mode(BLEND_MODE_ADD);
		m_render_target = m_canvas->get_texture();
		m_canvas->set_auto_clean(false);
		m_canvas->set_static(true);
		m_canvas->set_blend_mode(BLEND_MODE_MOD);
		set_ambient_color(Ness::Color::BLACK);
	}

	void LightNode::add(const RenderablePtr& object)
	{
		BaseNode::add(object);
		object->set_blend_mode(BLEND_MODE_ADD);
		m_need_update = true;
	}

	void LightNode::remove(const RenderablePtr& object)
	{
		BaseNode::remove(object);
		m_need_update = true;
	}

	LightPtr LightNode::create_light(const std::string& lightTexture, const Color& color)
	{
		LightPtr NewSprite = NESS_MAKE_PTR<Light>(this->m_renderer, lightTexture, color);
		add(NewSprite);
		return NewSprite;
	}

	void LightNode::render(const CameraPtr& camera)
	{
		// if invisible skip
		if (!m_visible)
			return;

		// if always-update is set to true
		if (m_always_update)
			m_need_update = true;

		// check if need update
		for (unsigned int i = 0; ((i < m_entities.size()) && m_need_update == false); i++)
		{
			if (m_entities[i]->need_transformations_update())
			{
				m_need_update = true;
			}
		}
		
		// if got here and don't need update, only render the canvas and return
		if (!m_need_update)
		{
			m_canvas->render();
			return;
		}

		// if got here it means we need to update the light node
		// clear canvas
		m_canvas->clear();

		// render all lights
		m_renderer->set_render_target(m_canvas->get_texture());
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			m_entities[i]->render(camera);
		}
		m_renderer->reset_render_target();

		// render the canvas layer
		m_canvas->render();

		// no longer need update
		m_need_update = false;
	}
};