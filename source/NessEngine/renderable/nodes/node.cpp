#include "node.h"
#include <algorithm>
#include "all_nodes.h"
#include "../entities/all_entities.h"
#include "../../renderer/renderer.h"

namespace Ness
{

	NodePtr Node::create_node(bool add_immediatly)
	{
		NodePtr NewNode = ness_make_ptr<Node>(this->m_renderer);
		if (add_immediatly) add(NewNode);
		return NewNode;
	}

	ZNodePtr Node::create_znode(bool add_immediatly)
	{
		ZNodePtr NewNode = ness_make_ptr<ZNode>(this->m_renderer);
		if (add_immediatly) add(NewNode);
		return NewNode;
	}

	StaticNodePtr Node::create_static_node(bool add_immediatly)
	{
		StaticNodePtr NewNode = ness_make_ptr<StaticNode>(this->m_renderer);
		if (add_immediatly) add(NewNode);
		return NewNode;
	}

	LightNodePtr Node::create_light_node(bool add_immediatly)
	{
		LightNodePtr NewNode = ness_make_ptr<LightNode>(this->m_renderer);
		add(NewNode);
		return NewNode;
	}

	SpritePtr Node::create_sprite(const String& textureName, bool add_immediatly)
	{
		SpritePtr NewSprite = ness_make_ptr<Sprite>(this->m_renderer, textureName);
		if (add_immediatly) add(NewSprite);
		return NewSprite;
	}

	TextPtr Node::create_text(const String& fontFile, const String& text, int fontSize, bool add_immediatly)
	{
		TextPtr NewSprite = ness_make_ptr<Text>(this->m_renderer, fontFile, text, fontSize);
		if (add_immediatly) add(NewSprite);
		return NewSprite;
	}

	MultiTextPtr Node::create_multitext(const String& fontFile, const String& text, int fontSize, bool add_immediatly)
	{
		MultiTextPtr NewSprite = ness_make_ptr<MultiText>(this->m_renderer, fontFile, text, fontSize);
		if (add_immediatly) add(NewSprite);
		return NewSprite;
	}

	RectangleShapePtr Node::create_rectangle(bool add_immediatly)
	{
		RectangleShapePtr NewSprite = ness_make_ptr<RectangleShape>(this->m_renderer);
		if (add_immediatly) add(NewSprite);
		return NewSprite;
	}

	CanvasPtr Node::create_canvas(const String& textureName, const Sizei& size, bool add_immediatly)
	{
		CanvasPtr NewSprite = ness_make_ptr<Canvas>(this->m_renderer, textureName, size);
		if (add_immediatly) add(NewSprite);
		return NewSprite;
	}

	TileMapPtr Node::create_tilemap(const String& spriteFile, const Sizei& mapSize, const Size& singleTileSize, const Size& tilesDistance, bool add_immediatly)
	{
		TileMapPtr NewMap = ness_make_ptr<TileMap>(this->m_renderer, spriteFile, mapSize, singleTileSize, tilesDistance);
		if (add_immediatly) add(NewMap);
		return NewMap;
	}

	 NodesMapPtr Node::create_nodesmap(const Sizei& mapSize, const Size& singleNodeSize, const Size& nodesDistance, bool add_immediatly)
	 {
		NodesMapPtr NewMap = ness_make_ptr<NodesMap>(this->m_renderer, mapSize, singleNodeSize, nodesDistance);
		if (add_immediatly) add(NewMap);
		return NewMap;
	 }
};