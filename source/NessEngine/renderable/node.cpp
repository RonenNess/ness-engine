#include "node.h"
#include <algorithm>
#include "sprite.h"
#include "tile_map.h"
#include "znode.h"
#include "shapes.h"
#include "../renderer/renderer.h"
#include <algorithm>

namespace Ness
{

	void Node::add(const NodePtr& object)
	{
		m_nodes.push_back(object);
		object->__change_parent(this);
	}

	void Node::add(const RenderablePtr& object)
	{
		m_entities.push_back(object);
		object->__change_parent(this);
	}

	void Node::__get_visible_entities(std::vector<Renderable*>& out_list, const CameraPtr& camera)
	{
		// add all son nodes
		for (unsigned int i = 0; i < m_nodes.size(); i++)
		{
			RenderableParent* current = m_nodes[i].get();
			current->__get_visible_entities(out_list, camera);
		}

		// add all the visible sprites
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			// get current sprite
			Renderable* current = m_entities[i].get();
			if (!current->is_really_visible(camera))
				continue;

			// add to rendering list
			out_list.push_back(current);
		}
	}

	void Node::remove(const NodePtr& object)
	{
		m_nodes.erase(std::remove(m_nodes.begin(), m_nodes.end(), object), m_nodes.end());
		object->__change_parent(nullptr);
	}

	void Node::remove(const RenderablePtr& object)
	{
		m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), object), m_entities.end());
		object->__change_parent(nullptr);
	}

	bool Node::is_really_visible(const CameraPtr& camera)
	{
		// is this node even visible?
		if (!m_visible)
			return false;

		// check all sprites
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			if (m_entities[i]->is_really_visible(camera))
				return true;
		}

		// check all sub nodes
		for (unsigned int i = 0; i < m_nodes.size(); i++)
		{
			if (m_nodes[i]->is_really_visible(camera))
				return true;
		}

		// if got here it means the node is not visible
		return false;
	}

	NodePtr Node::create_node()
	{
		NodePtr NewNode = std::make_shared<Node>(this->m_renderer, this);
		add(NewNode);
		return NewNode;
	}

	NodePtr Node::create_znode()
	{
		NodePtr NewNode = std::make_shared<ZNode>(this->m_renderer, this);
		add(NewNode);
		return NewNode;
	}

	SpritePtr Node::create_sprite(const std::string& textureName)
	{
		SpritePtr NewSprite = std::make_shared<Sprite>(this->m_renderer, this, textureName);
		add(NewSprite);
		return NewSprite;
	}

	RectangleShapePtr Node::create_rectangle()
	{
		RectangleShapePtr NewSprite = std::make_shared<RectangleShape>(this->m_renderer, this);
		add(NewSprite);
		return NewSprite;
	}

	CanvasPtr Node::create_canvas(const std::string& textureName, const Sizei& size)
	{
		CanvasPtr NewSprite = std::make_shared<Canvas>(this->m_renderer, this, textureName, size);
		add(NewSprite);
		return NewSprite;
	}

	TileMapPtr Node::create_tilemap(const std::string& spriteFile, Sizei mapSize, Size singleTileSize)
	{
		TileMapPtr NewMap(new TileMap(this->m_renderer, this, spriteFile, mapSize, singleTileSize));
		add(NewMap);
		return NewMap;
	}

	const SRenderTransformations& Node::get_absolute_transformations()
	{

		// if need to update transformations from parent, do it
		if (m_need_trans_update)
		{
			// begin with this node basic transformations and add parent's transformations
			m_absolute_trans = m_transformations;
			m_absolute_trans.add_transformations(m_parent->get_absolute_transformations());

			// set no longer need update
			m_need_trans_update = false;
		}

		// return the cached absolute transformations
		return m_absolute_trans;
	}

	void Node::transformations_update()
	{
		m_need_trans_update = true;
		for (unsigned int i = 0; i < m_nodes.size(); i++)
		{
			m_nodes[i]->transformations_update();
		}
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			m_entities[i]->transformations_update();
		}
	}

	void Node::render(const CameraPtr& camera)
	{
		// if invisible skip
		if (!m_visible)
			return;

		// if there is alternative target texture
		if (m_render_target)
		{
			m_renderer->set_render_target(m_render_target);
		}

		// render all sprites
		for (unsigned int i = 0; i < m_entities.size(); i++)
		{
			m_entities[i]->render(camera);
		}

		// render all sub nodes
		for (unsigned int i = 0; i < m_nodes.size(); i++)
		{
			m_nodes[i]->render(camera);
		}

		// remove target texture
		if (m_render_target)
		{
			m_renderer->reset_render_target();
		}
	}
};