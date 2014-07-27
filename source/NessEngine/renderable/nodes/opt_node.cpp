#include "opt_node.h"
#include <algorithm>
#include "../../renderer/renderer.h"

namespace Ness
{

	// render everything, with z order!
	void OptNode::render(const CameraPtr& camera)
	{
		// if automatic updates are on, try to update
		if (m_update_automatically)
		{
			update_batches(true);
		}

		// create the ordered render list
		static Vector<RenderableAPI*> render_list;
		render_list.clear();

		// add all the visible sprites
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			// if need to break entities of son nodes:
			if (m_break_groups)
			{
				// check if current entity is indeed a node, and if so, break it
				NodeAPI* currentNode = dynamic_cast<NodeAPI*>(m_entities[i].get());
				if (currentNode)
				{
					currentNode->__get_visible_entities(render_list, camera);
					continue;
				}
			}

			// if got here it's either a sprite entity or a node but we want to keep groups. add it to rendering list if visible
			RenderableAPI* current = m_entities[i].get();
			if (!current->is_really_visible(camera))
				continue;
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