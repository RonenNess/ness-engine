#include "node.h"
#include <algorithm>
#include "sprite.h"
#include "tile_map.h"
#include "znode.h"
#include "../renderer/renderer.h"
#include <algorithm>

namespace Ness
{

	void Node::add(const NodePtr& object)
	{
		m_nodes.push_back(object);
		object->__change_parent(this);
	}

	void Node::add(const SpritePtr& object)
	{
		m_sprites.push_back(object);
		object->__change_parent(this);
	}
	
	void Node::add(const TileMapPtr& object)
	{
		m_sprites.insert(m_sprites.begin(), object);
		object->__change_parent(this);
	}

	void Node::remove(const NodePtr& object)
	{
		m_nodes.erase(std::remove(m_nodes.begin(), m_nodes.end(), object), m_nodes.end());
		object->__change_parent(nullptr);
	}

	void Node::remove(const SpritePtr& object)
	{
		m_sprites.erase(std::remove(m_sprites.begin(), m_sprites.end(), object), m_sprites.end());
		object->__change_parent(nullptr);
	}

	void Node::remove(const TileMapPtr& object)
	{
		m_sprites.erase(std::remove(m_sprites.begin(), m_sprites.end(), object), m_sprites.end());
		object->__change_parent(nullptr);
	}

	bool Node::is_really_visible(const CameraPtr& camera)
	{
		// is this node even visible?
		if (!m_visible)
			return false;

		// check all sprites
		for (unsigned int i = 0; i < m_sprites.size(); i++)
		{
			if (m_sprites[i]->is_really_visible(camera))
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
		for (unsigned int i = 0; i < m_sprites.size(); i++)
		{
			m_sprites[i]->transformations_update();
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

		// render all sub nodes
		for (unsigned int i = 0; i < m_nodes.size(); i++)
		{
			m_nodes[i]->render(camera);
		}

		// render all sprites
		for (unsigned int i = 0; i < m_sprites.size(); i++)
		{
			m_sprites[i]->render(camera);
		}

		// remove target texture
		if (m_render_target)
		{
			m_renderer->reset_render_target();
		}
	}
};