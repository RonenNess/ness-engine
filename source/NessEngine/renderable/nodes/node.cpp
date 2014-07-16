#include "node.h"
#include <algorithm>
#include "all_nodes.h"
#include "../entities/all_entities.h"
#include "../../renderer/renderer.h"

namespace Ness
{

	NodePtr Node::create_node()
	{
		NodePtr NewNode = NESS_MAKE_PTR<Node>(this->m_renderer, this);
		add(NewNode);
		return NewNode;
	}

	ZNodePtr Node::create_znode()
	{
		ZNodePtr NewNode = NESS_MAKE_PTR<ZNode>(this->m_renderer, this);
		add(NewNode);
		return NewNode;
	}

	LightNodePtr Node::create_light_node()
	{
		LightNodePtr NewNode = NESS_MAKE_PTR<LightNode>(this->m_renderer, this);
		add(NewNode);
		return NewNode;
	}

	SpritePtr Node::create_sprite(const std::string& textureName)
	{
		SpritePtr NewSprite = NESS_MAKE_PTR<Sprite>(this->m_renderer, this, textureName);
		add(NewSprite);
		return NewSprite;
	}

	TextPtr Node::create_text(const std::string& fontFile, const std::string& text, int fontSize)
	{
		TextPtr NewSprite = NESS_MAKE_PTR<Text>(this->m_renderer, this, fontFile, text, fontSize);
		add(NewSprite);
		return NewSprite;
	}

	RectangleShapePtr Node::create_rectangle()
	{
		RectangleShapePtr NewSprite = NESS_MAKE_PTR<RectangleShape>(this->m_renderer, this);
		add(NewSprite);
		return NewSprite;
	}

	CanvasPtr Node::create_canvas(const std::string& textureName, const Sizei& size)
	{
		CanvasPtr NewSprite = NESS_MAKE_PTR<Canvas>(this->m_renderer, this, textureName, size);
		add(NewSprite);
		return NewSprite;
	}

	TileMapPtr Node::create_tilemap(const std::string& spriteFile, Sizei mapSize, Size singleTileSize)
	{
		TileMapPtr NewMap(new TileMap(this->m_renderer, this, spriteFile, mapSize, singleTileSize));
		add(NewMap);
		return NewMap;
	}
};