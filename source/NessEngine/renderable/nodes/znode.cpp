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

#include "znode.h"
#include <algorithm>
#include "../../renderer/renderer.h"

namespace Ness
{
	// function to sort by z index
	// algoritm: go by z index. if same z index, go by position.y. if same position.y, go by position.x.
	bool sort_by_z(SharedPtr<RenderableAPI>& a, SharedPtr<RenderableAPI>& b)
	{
		return a->get_zindex() < b->get_zindex();
	}
	bool sort_by_z_absolute(SharedPtr<RenderableAPI>& a, SharedPtr<RenderableAPI>& b)
	{
		return  a->get_absolute_zindex() < b->get_absolute_zindex();
	}

	// render everything, with z order!
	void ZNode::render(const CameraPtr& camera)
	{
		// create the ordered render list
		static RenderablesList render_list;

		// if its time to reorder, reset the render list and repopulate it
		if (m_time_until_next_zorder <= 0.0f)
		{
			m_time_until_next_zorder = m_update_list_intervals;
			render_list.clear();

			// add all the visible sprites
			for (unsigned int i = 0; i < m_entities.size(); i++)
			{
				// if need to break entities of son nodes:
				if (m_break_groups && m_entities[i]->is_node())
				{
					// check if current entity is indeed a node, and if so, break it
					SharedPtr<NodeAPI> currentNode = ness_ptr_cast<NodeAPI>(m_entities[i]);
					if (!currentNode->get_flag(Ness::RNF_NEVER_BREAK))
					{
						currentNode->__get_visible_entities(render_list, camera, true);
						continue;
					}
				}

				// if got here it's either a sprite entity or a node but we want to keep groups. add it to rendering list if visible
				SharedPtr<RenderableAPI>& current = m_entities[i];
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
		}
		// if not time to update render ist yet decrease the time
		else
		{
			m_time_until_next_zorder -= m_renderer->time_factor();
		}

		// set rendering target
		if (m_render_target)
		{
			m_renderer->push_render_target(m_render_target);
		}

		// render everything!
		for (unsigned int i = 0; i < render_list.size(); i++)
		{
			render_list[i]->render(camera);
		}

		// reset rendering target if such target was used
		if (m_render_target)
		{
			m_renderer->pop_render_target();
		}
	}
};