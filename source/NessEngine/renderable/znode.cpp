#include "znode.h"
#include <algorithm>
#include "../renderer/renderer.h"

namespace Ness
{
	// function to sort by z index
	// algoritm: go by z index. if same z index, go by position.y. if same position.y, go by position.x.
	bool sort_by_z(Renderable* a, Renderable* b)
	{
		if (a->get_zindex() == b->get_zindex())
		{
			if (a->get_position().y == b->get_position().y)
			{
				return a->get_position().x < b->get_position().x;
			}
			return a->get_position().y < b->get_position().y;
		}
		return a->get_zindex() < b->get_zindex();
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
			// get current sprite
			Renderable* current = m_nodes[i].get();
			if (!current->is_really_visible(camera))
				continue;

			render_list.push_back(current);
		}

		// add all the visible sprites
		for (unsigned int i = 0; i < m_sprites.size(); i++)
		{
			// get current sprite
			Renderable* current = m_sprites[i].get();
			if (!current->is_really_visible(camera))
				continue;

			// if it's a tilemap, we have a special case (adding all the tiles to the render list to include them in the zorder)
			TileMap* tiles = dynamic_cast<TileMap*>(current);
			if (tiles)
			{
				Rectangle range = tiles->get_tiles_in_screen(camera);
				Pointi index;
				for (index.x = range.x; index.x < range.w; index.x++)
				{
					for (index.y = range.y; index.y < range.h; index.y++)
					{
						render_list.push_back(tiles->get_sprite(index).get());
					}
				}
			}
			// for anything else simply add to rendering list
			else
			{
				render_list.push_back(current);
			}
		}

		// sort based on z!
		std::sort(render_list.begin(), render_list.end(), sort_by_z);

		if (m_render_target)
		{
			m_renderer->set_render_target(m_render_target);
		}

		// render everything!
		for (unsigned int i = 0; i < render_list.size(); i++)
		{
			render_list[i]->render(camera);
		}

		if (m_render_target)
		{
			m_renderer->reset_render_target();
		}
	}
};