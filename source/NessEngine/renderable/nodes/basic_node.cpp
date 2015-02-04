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

#include "basic_node.h"
#include <algorithm>
#include "all_nodes.h"
#include "../entities/all_entities.h"
#include "../../renderer/renderer.h"

namespace Ness
{

	void BaseNode::__get_visible_entities(RenderablesList& out_list, const CameraApiPtr& camera, bool break_son_nodes)
	{
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			SharedPtr<RenderableAPI>& curr = m_entities[i];

			// check if current entity is a node
			if (curr->is_node())
			{
				if (break_son_nodes && !curr->get_flag(RNF_NEVER_BREAK))
				{
					SharedPtr<NodeAPI>& currentNode = ness_ptr_cast<NodeAPI>(curr);
					currentNode->__get_visible_entities(out_list, camera, break_son_nodes);
				}
				else
				{
					out_list.push_back(curr);
				}
			}
			// if not a node, check if in screen and if so add it
			else
			{
				if (!curr->is_really_visible(camera))
					continue;

				// add to rendering list
				out_list.push_back(curr);
			}
		}
	}

	bool BaseNode::was_rendered_this_frame() const
	{
		return m_renderer->get_frameid() == m_last_render_frame_id;
	}

	bool BaseNode::was_updated_this_frame() const
	{
		return m_renderer->get_frameid() == m_last_update_frame_id;
	}

	void BaseNode::destroy()
	{
		clear();
	}

	void BaseNode::clear()
	{
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			m_entities[i]->__change_parent(nullptr);
		}
		m_entities.clear();
	}

	void BaseNode::select_entities_from_position(EntitiesList& out_list, const Pointf& pos, bool recursive) const
	{
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			// get current son
			const SharedPtr<RenderableAPI>& curr = m_entities[i];

			// if not selectable skip
			if (!curr->get_flag(RNF_SELECTABLE))
				continue;

			// check if current entity is a node and should scan it
			if (curr->is_node())
			{
				if (recursive)
				{
					SharedPtr<NodeAPI>& currentNode = ness_ptr_cast<NodeAPI>(curr);
					currentNode->select_entities_from_position(out_list, pos, recursive);
				}
			}
			// if not a node, check if touches position and if so add it to the list
			else
			{
				const SharedPtr<EntityAPI>& curr_entity = ness_ptr_cast<EntityAPI>(m_entities[i]);
				if (curr_entity->touch_point(pos))
					out_list.push_back(curr_entity);
			}
		}
	}

	void BaseNode::__get_all_entities(RenderablesList& out_list, bool breakGroups)
	{
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			// if need to break son nodes:
			if (breakGroups)
			{
				if (m_entities[i]->is_node())
				{
					SharedPtr<NodeAPI> currentNode = ness_ptr_cast<NodeAPI>(m_entities[i]);
					currentNode->__get_all_entities(out_list, breakGroups);
				}
				else
				{
					out_list.push_back(m_entities[i]);
				}
			}
			// if not a node, check if in screen and if so add it
			else
			{
				out_list.push_back(m_entities[i]);
			}
		}
	}

	void BaseNode::add(const RenderablePtr& object)
	{
		m_entities.push_back(object);
		object->__change_parent(this);
	}

	void BaseNode::add_first(const RenderablePtr& object)
	{
		m_entities.insert(m_entities.begin(), object);
		object->__change_parent(this);
	}

	void BaseNode::remove(const RenderablePtr& object)
	{
		object->__change_parent(nullptr);
		m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), object), m_entities.end());
	}

	bool BaseNode::is_really_visible(const CameraApiPtr& camera)
	{
		// is this node even visible?
		if (!m_visible || get_absolute_transformations_const().color.a <= 0)
			return false;

		// check all sprites
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			if (m_entities[i]->is_really_visible(camera))
				return true;
		}

		// if got here it means the node is not visible
		return false;
	}

	const SRenderTransformations& BaseNode::get_absolute_transformations()
	{

		// if need to update transformations from parent, do it
		if (m_need_trans_update)
		{
			// begin with this node basic transformations and add parent's transformations
			m_absolute_trans = m_transformations;
			if (m_parent) m_absolute_trans.add_transformations(m_parent->get_absolute_transformations());

			// set no longer need update
			m_need_trans_update = false;
		}

		// return the cached absolute transformations
		return m_absolute_trans;
	}

	void BaseNode::set_render_target(const ManagedResources::ManagedTexturePtr& NewTarget) 
	{
		m_render_target = NewTarget;
	}

	void BaseNode::set_render_target(const CanvasPtr& NewTarget) 
	{
		if (NewTarget->parent() == this)
		{
			throw IllegalAction("Cannot set node render target that is a direct son of the node!");
		}
		m_render_target = NewTarget->get_texture();
	}

	void BaseNode::remove_render_target() 
	{
		m_render_target.reset();
	}

	void BaseNode::transformations_update()
	{
		m_need_trans_update = true;
		m_last_update_frame_id = m_renderer->get_frameid();
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			Transformable* current = dynamic_cast<Transformable*>(m_entities[i].get());
			if (current)
			{
				current->transformations_update();
			}
		}
	}

	RenderablePtr BaseNode::get_son(const String& name)
	{
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			if (m_entities[i]->get_name() == name)
				return m_entities[i];
		}
		return RenderablePtr();
	}

	void BaseNode::render()
	{
		render(m_renderer->get_null_camera());
	}

	void BaseNode::render(const CameraApiPtr& camera)
	{
		// if invisible skip
		if (!m_visible)
			return;

		// if there is alternative target texture
		if (m_render_target)
		{
			m_renderer->push_render_target(m_render_target);
		}

		// lastly rendered
		m_last_render_frame_id = m_renderer->get_frameid();

		// render all son entities
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			m_entities[i]->render(camera);
		}

		// remove target texture
		if (m_render_target)
		{
			m_renderer->pop_render_target();
		}
	}
};