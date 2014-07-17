#include "node.h"
#include <algorithm>
#include "all_nodes.h"
#include "../entities/all_entities.h"
#include "../../renderer/renderer.h"

namespace Ness
{

	NodePtr Node::create_node()
	{
		NodePtr NewNode = ness_make_ptr<Node>(this->m_renderer);
		add(NewNode);
		return NewNode;
	}

	ZNodePtr Node::create_znode()
	{
		ZNodePtr NewNode = ness_make_ptr<ZNode>(this->m_renderer);
		add(NewNode);
		return NewNode;
	}

	LightNodePtr Node::create_light_node()
	{
		LightNodePtr NewNode = ness_make_ptr<LightNode>(this->m_renderer);
		add(NewNode);
		return NewNode;
	}

	SpritePtr Node::create_sprite(const String& textureName)
	{
		SpritePtr NewSprite = ness_make_ptr<Sprite>(this->m_renderer, textureName);
		add(NewSprite);
		return NewSprite;
	}

	TextPtr Node::create_text(const String& fontFile, const String& text, int fontSize)
	{
		TextPtr NewSprite = ness_make_ptr<Text>(this->m_renderer, fontFile, text, fontSize);
		add(NewSprite);
		return NewSprite;
	}

	MultiTextPtr Node::create_multitext(const String& fontFile, const String& text, int fontSize)
	{
		MultiTextPtr NewSprite = ness_make_ptr<MultiText>(this->m_renderer, fontFile, text, fontSize);
		add(NewSprite);
		return NewSprite;
	}

	RectangleShapePtr Node::create_rectangle()
	{
		RectangleShapePtr NewSprite = ness_make_ptr<RectangleShape>(this->m_renderer);
		add(NewSprite);
		return NewSprite;
	}

	CanvasPtr Node::create_canvas(const String& textureName, const Sizei& size)
	{
		CanvasPtr NewSprite = ness_make_ptr<Canvas>(this->m_renderer, textureName, size);
		add(NewSprite);
		return NewSprite;
	}

	TileMapPtr Node::create_tilemap(const String& spriteFile, Sizei mapSize, Size singleTileSize)
	{
		TileMapPtr NewMap(new TileMap(this->m_renderer, spriteFile, mapSize, singleTileSize));
		add(NewMap);
		return NewMap;
	}
};