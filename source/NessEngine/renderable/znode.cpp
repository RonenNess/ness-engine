#include "znode.h"
#include <algorithm>
#include "../renderer/renderer.h"

namespace Ness
{
	// function to sort by z index
	// algoritm: go by z index. if same z index, go by position.y. if same position.y, go by position.x.
	bool sort_by_z(Renderable* a, Renderable* b)
	{
		return a->get_zindex() < b->get_zindex();
	}
	bool sort_by_z_absolute(Renderable* a, Renderable* b)
	{
		return  a->get_absolute_transformations().zorder < b->get_absolute_transformations().zorder;
	}

	// render everything, with z order!
	void ZNode::render(const CameraPtr& camera)
	{
		// create the ordered render list
		static std::vector<Renderable*> render_list;
		render_list.clear();

		// add all the visible nodes
		for (unsigned int i = 0; i < m_nodes.size(); i++)
		{

			// if break groups, insert all entities in son node into the rendering list
			if (m_break_groups)
			{
				RenderableParent* current = m_nodes[i].get();
				current->__get_visible_entities(render_list);
			}
			// if don't break groups, treat this node like a normal entity
			else
			{
				Renderable* current = m_nodes[i].get();
				if (!current->is_really_visible(camera))
					continue;
				render_list.push_back(current);
			}
		}

		// add all the visible sprites
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			// get current sprite
			Renderable* current = m_entities[i].get();
			if (!current->is_really_visible(camera))
				continue;

			// add to rendering list
			render_list.push_back(current);
		}

		// sort based on z!
		// if break groups, we need to sort by absolute z ordering
		if (m_break_groups)
		{
			std::sort(render_list.begin(), render_list.end(), sort_by_z_absolute);
		}
		// if keep groups, ordering by relative z index is enough
		else
		{
			std::sort(render_list.begin(), render_list.end(), sort_by_z);
		}

		// set rendering target
		if (m_render_target)
		{
			m_renderer->set_render_target(m_render_target);
		}

		// render everything!
		for (unsigned int i = 0; i < render_list.size(); i++)
		{
			render_list[i]->render(camera);
		}

		// reset rendering target if such target was used
		if (m_render_target)
		{
			m_renderer->reset_render_target();
		}
	}
};