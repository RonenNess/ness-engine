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

#include "../../renderer/renderer.h"
#include "nodes_map.h"

namespace Ness
{
	const SRenderTransformations& NodesMap::get_absolute_transformations()
	{
		// if don't have a parent, return self transformations
		if (!m_parent)
			return m_transformations;

		// calculate this transformations with parent transformations
		m_absolute_transformations = m_transformations;
		m_absolute_transformations.add_transformations(m_parent->get_absolute_transformations());
		return m_absolute_transformations;
	}

	NodesMap::NodesMap(Renderer* renderer, const Sizei& mapSize, const Size& nodesSize, const Size& nodesDistance, 
		TCreateNodes createNodesFunction, bool overridePositionAndZ)
		: NodeAPI(renderer), m_size(mapSize), m_nodes_distance(nodesDistance),  
		m_node_size(nodesSize), m_extra_tiles_factor(0, 0)
	{
		if (m_nodes_distance == Size::ZERO)
			m_nodes_distance = m_node_size;

		// create the sprites grid
		Pointi index;
		m_nodes.resize(m_size.x);
		for (index.x = 0; index.x < m_size.x; index.x++)
		{
			m_nodes[index.x].resize(m_size.y);
			for (index.y = 0; index.y < m_size.y; index.y++)
			{
				// create the sprite
				NodeAPIPtr NewNode;
				if (createNodesFunction == nullptr)
				{
					NewNode = ness_make_ptr<Node>(this->m_renderer);
				}
				else
				{
					NewNode = createNodesFunction(index);
				}
				NewNode->__change_parent(this);
				
				// arrange current tile in the grid
				if (overridePositionAndZ || createNodesFunction == nullptr)
				{
					arrange_node(NewNode, index);
				}

				// add to matrix of tiles
				m_nodes[index.x][index.y] = NewNode;
			}
		}
	}

	// arrange a single tile sprite during creation
	void NodesMap::arrange_node(const NodeAPIPtr& node, const Ness::Pointi& index)
	{				
		// set position
		node->set_position(Point(
			(float)(index.x * m_nodes_distance.x), 
			(float)(index.y * m_nodes_distance.y)));

		// set z-index
		node->set_zindex(node->get_position().y);
	}

	void NodesMap::apply_to_all(TExecuteOnNodes func)
	{
		Sizei index;
		for (index.x = 0; index.x < m_size.x; index.x++)
		{
			for (index.y = 0; index.y < m_size.y; index.y++)
			{
				func(index, m_nodes[index.x][index.y]);
			}
		}
	}

	void NodesMap::transformations_update()
	{
		Sizei index;
		for (index.x = 0; index.x < m_size.x; index.x++)
		{
			for (index.y = 0; index.y < m_size.y; index.y++)
			{
				m_nodes[index.x][index.y]->transformations_update();
			}
		}
	}

	void NodesMap::put_in_range(int& i, int& j)
	{
		if (i < 0) i = 0;
		if (i > m_size.x) i = m_size.x ;
		if (j < 0) j = 0;
		if (j > m_size.y) j = m_size.y;
	}

	int NodesMap::get_first_tile_in_screen_x(const Ness::Point& cameraPos)
	{
		float scale = get_absolute_transformations().scale.x;
		return cameraPos.x < 0 ? (int)(((-cameraPos.x - (m_node_size.x * scale)) / (m_nodes_distance.x * scale))) - m_extra_tiles_factor.x : 0;
	}

	int NodesMap::get_first_tile_in_screen_y(const Ness::Point& cameraPos)
	{
		float scale = get_absolute_transformations().scale.y;
		return cameraPos.y < 0 ? (int)(((-cameraPos.y - (m_node_size.y * scale)) / (m_nodes_distance.y * scale))) - m_extra_tiles_factor.y : 0;
	}

	int NodesMap::get_tiles_in_screen_x()
	{
		float scale = get_absolute_transformations().scale.x;
		return (int)((m_renderer->get_target_size().x + (m_node_size.x * scale)) / (m_nodes_distance.x * scale)) + 2 + m_extra_tiles_factor.x * 2;
	}

	int NodesMap::get_tiles_in_screen_y()
	{
		float scale = get_absolute_transformations().scale.y;
		return (int)((m_renderer->get_target_size().y + (m_node_size.y * scale)) / (m_nodes_distance.y * scale)) + 2 + m_extra_tiles_factor.y * 2;
	}

	void NodesMap::__get_visible_entities(RenderablesList& out_list, const CameraPtr& camera, bool break_son_nodes)
	{
		Rectangle range = get_nodes_in_screen(camera);
		for (int i = range.x; i < range.w; i++)
		{
			for (int j = range.y; j < range.h; j++)
			{
				NodeAPIPtr& curr = m_nodes[i][j];
				if (break_son_nodes && !curr->get_flag(RNF_NEVER_BREAK))
				{
					curr->__get_visible_entities(out_list, camera, break_son_nodes);
				}
				else
				{
					out_list.push_back(curr);
				}
			}
		}
	}

	void NodesMap::select_entities_from_position(EntitiesList& out_list, const Pointf& pos, bool recursive) const
	{
		if (!recursive)
			return;

		const NodeAPIPtr& node = ((NodesMap*)(this))->get_node_by_position_any(pos);
		if (node && node->get_flag(RNF_SELECTABLE))
		{
			node->select_entities_from_position(out_list, pos, recursive);
		}
	}
	
	void NodesMap::__get_all_entities(RenderablesList& out_list, bool breakGroups)
	{
		for (int i = 0; i < m_size.x; i++)
		{
			for (int j = 0; j < m_size.y; j++)
			{
				out_list.push_back(m_nodes[i][j]);
			}
		}
	}

	void NodesMap::destroy()
	{
		Sizei index;
		for (index.x = 0; index.x < m_size.x; index.x++)
		{
			for (index.y = 0; index.y < m_size.y; index.y++)
			{
				m_nodes[index.x][index.y]->__change_parent(nullptr);
			}
		}
		m_nodes.clear();
	}

	Rectangle NodesMap::get_nodes_in_screen(const CameraPtr& camera) 
	{
		Rectangle ret;
		SRenderTransformations trans = get_absolute_transformations(); 

		Point pos = trans.position;
		if (camera)
		{
			pos.x -= camera->position.x;
			pos.y -= camera->position.y;
		}

		ret.x = get_first_tile_in_screen_x(pos);
		ret.y = get_first_tile_in_screen_y(pos);
		ret.w = ret.x + get_tiles_in_screen_x();
		ret.h = ret.y + get_tiles_in_screen_y();
		
		put_in_range(ret.x, ret.y);
		put_in_range(ret.w, ret.h);

		return ret;
	}

	Pointi NodesMap::get_index_from_position(const Point& position)
	{
		Pointi index;
		Point scale = get_absolute_transformations().scale;
		index.x = (int)std::floor((position.x) / (m_nodes_distance.x * scale.x));
		index.y = (int)std::floor((position.y) / (m_nodes_distance.y * scale.y));
		return index;
	}

	Pointi NodesMap::get_index_from_position(const Point& position) const
	{
		Pointi index;
		Point scale = m_transformations.scale;
		index.x = (int)std::floor((position.x) / (m_nodes_distance.x * scale.x));
		index.y = (int)std::floor((position.y) / (m_nodes_distance.y * scale.y));
		return index;
	}

	Point NodesMap::get_position_from_index(const Pointi& index) const
	{
		return m_nodes[index.x][index.y]->get_position();
	}

	Rectangle NodesMap::get_occupied_region() const
	{
		Rectangle ret;
		ret.x = (int)m_absolute_transformations.position.x;
		ret.y = (int)m_absolute_transformations.position.y;
		ret.w = (int)(((m_size.x - 1) * m_nodes_distance.x) + m_node_size.x);
		ret.h = (int)(((m_size.y - 1) * m_nodes_distance.y) + m_node_size.y);
		return ret;
	}

	NodeAPIPtr NodesMap::get_node_by_position_any(const Point& position)
	{
		static NodeAPIPtr empty;
		Pointi index = get_index_from_position(position);
		if (index.x < 0 || index.y < 0 || index.x >= m_size.x || index.y >= m_size.y)
		{
			return empty;
		}
		return get_node_any(index);
	}

	NodeAPIPtr NodesMap::get_node_by_position_any(const Point& position) const
	{
		static NodeAPIPtr empty;
		Pointi index = get_index_from_position(position);
		if (index.x < 0 || index.y < 0 || index.x >= m_size.x || index.y >= m_size.y)
		{
			return empty;
		}
		return get_node_any(index);
	}

	bool NodesMap::is_really_visible(const CameraPtr& camera)
	{
		if (!m_visible)
			return false;

		SRenderTransformations trans = get_absolute_transformations(); 
		if (trans.color.a <= 0.0f)
			return false;

		Rectangle TileInScreen = get_nodes_in_screen(camera);
		if (TileInScreen.x >= TileInScreen.w || TileInScreen.y >= TileInScreen.h)
			return false;

		return true;
	}

	void NodesMap::render(const CameraPtr& camera)
	{
		// if invisible skip
		if (!m_visible)
			return;

		Rectangle range = get_nodes_in_screen(camera);

		// render all visible tiles
		for (int i = range.x; i < range.w; i++)
		{
			for (int j = range.y; j < range.h; j++)
			{
				m_nodes[i][j]->render(camera);
			}
		}
	}

};