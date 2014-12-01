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

#include "shadow_node.h"
#include "../../renderer/renderer.h"
#include "../entities/canvas.h"

namespace Ness
{
	ShadowNode::ShadowNode(Renderer* renderer) : BaseNode(renderer), m_need_update(true), m_always_update(false)
	{
		// create the canvas.
		// we will render everything on the canvas as additive, and then render the canvas itself with mod blend
		// note: canvas clear color will represent the ambient color, i.e. the color of shadow when there's no shadowing.
		static unsigned int uniqueId = 0;
		m_canvas = ness_make_ptr<Canvas>(this->m_renderer, String("shadow_node_") + ness_to_string((long long)uniqueId));
		uniqueId++;
		m_render_target = m_canvas->get_texture();
		m_canvas->set_auto_clean(false);
		m_canvas->set_static(true);
		m_canvas->set_anchor(Ness::Point::ZERO);
		m_canvas->set_blend_mode(BLEND_MODE_MOD);
		set_ambient_shadow(Ness::Color::WHITE);

		// set default flag in case this node be put inside a z-node
		set_flag(Ness::RNF_NEVER_BREAK);
	}

	void ShadowNode::get_shadows_in_screen(Containers::Vector<ShadowPtr>& out_list, const CameraPtr& camera) const
	{
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			ShadowPtr curr = ness_ptr_cast<Shadow>(m_entities[i]);
			if (curr->is_really_visible_const(camera))
			{
				out_list.push_back(curr);
			}
		}
	}

	void ShadowNode::add(const RenderablePtr& object)
	{
		if (ness_ptr_cast<Shadow>(object) == nullptr)
		{
			throw IllegalAction("Can only add shadows to a shadow node!");
		}
		BaseNode::add(object);
		m_need_update = true;
	}

	void ShadowNode::transformations_update()
	{
		BaseNode::transformations_update();
		m_need_update = true;
	}

	void ShadowNode::remove(const RenderablePtr& object)
	{
		BaseNode::remove(object);
		m_need_update = true;
	}

	ShadowPtr ShadowNode::create_shadow(const String& shadowTexture, const Color& color)
	{
		ShadowPtr NewSprite = ness_make_ptr<Shadow>(this->m_renderer, shadowTexture, color);
		add(NewSprite);
		return NewSprite;
	}

	void ShadowNode::render(const CameraPtr& camera)
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
		// basically check if there's a Shadow that need transformations update + is really visible and within screen.
		// stop the loop when looping all objects or once need update is true.
		for (unsigned int i = 0; ((i < m_entities.size()) && !m_need_update); i++)
		{
			m_need_update = ness_ptr_cast<Shadow>(m_entities[i])->need_redraw();
		}
		
		// if got here and don't need update, only render the canvas and return
		if (!m_need_update)
		{
			m_canvas->render();
			return;
		}

		// if got here it means we need to update the Shadow node
		// clear canvas
		m_canvas->clear();

		// render all shadows
		m_renderer->push_render_target(m_canvas->get_texture());
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			m_entities[i]->render(camera);
			ness_ptr_cast<Shadow>(m_entities[i])->set_need_redraw(false);
		}
		m_renderer->pop_render_target();

		// render the canvas layer
		m_canvas->render();

		// no longer need update
		m_need_update = false;
	}
};