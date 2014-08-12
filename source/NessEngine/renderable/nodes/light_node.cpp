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
		m_canvas = ness_make_ptr<Canvas>(this->m_renderer, String("light_node_") + ness_to_string((long long)uniqueId));
		uniqueId++;
		set_blend_mode(BLEND_MODE_ADD);
		m_render_target = m_canvas->get_texture();
		m_canvas->set_auto_clean(false);
		m_canvas->set_static(true);
		m_canvas->set_anchor(Ness::Point::ZERO);
		m_canvas->set_blend_mode(BLEND_MODE_MOD);
		set_ambient_color(Ness::Color::BLACK);
	}

	void LightNode::get_lights_in_screen(Ness::Vector<LightPtr>& out_list, const CameraPtr& camera) const
	{
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			LightPtr curr = ness_ptr_cast<Light>(m_entities[i]);
			if (curr->is_really_visible_const(camera))
			{
				out_list.push_back(curr);
			}
		}
	}

	void LightNode::add(const RenderablePtr& object)
	{
		BaseNode::add(object);
		object->set_blend_mode(BLEND_MODE_ADD);
		m_need_update = true;
	}

	void LightNode::transformations_update()
	{
		BaseNode::transformations_update();
		m_need_update = true;
	}

	void LightNode::remove(const RenderablePtr& object)
	{
		BaseNode::remove(object);
		m_need_update = true;
	}

	LightPtr LightNode::create_light(const String& lightTexture, const Color& color)
	{
		LightPtr NewSprite = ness_make_ptr<Light>(this->m_renderer, lightTexture, color);
		add(NewSprite);
		return NewSprite;
	}

	void LightNode::render(const CameraPtr& camera)
	{
		// if invisible skip
		if (!m_visible)
			return;

		// if always-update is set to true:
		if (m_always_update)
			m_need_update = true;
		else
		{
			if (camera && camera->position != m_last_camera_pos)
			{
				m_need_update = true;
				m_last_camera_pos = camera->position;
			}
		}

		// check if need update
		// basically check if there's a light that need transformations update + is really visible and within screen.
		// stop the loop when looping all objects or once need update is true.
		for (unsigned int i = 0; ((i < m_entities.size()) && !m_need_update); i++)
		{
			if (ness_ptr_cast<Light>(m_entities[i])->need_redraw())
			{
				m_need_update = m_entities[i]->is_really_visible(camera);
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
		m_renderer->push_render_target(m_canvas->get_texture());
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			m_entities[i]->render(camera);
			ness_ptr_cast<Light>(m_entities[i])->set_need_redraw(false);
		}
		m_renderer->pop_render_target();

		// render the canvas layer
		m_canvas->render();

		// no longer need update
		m_need_update = false;
	}
};