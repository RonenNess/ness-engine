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

	ParticlesNodePtr Node::create_particles_node(const Size& EstimatedSize, bool add_immediatly)
	{
		ParticlesNodePtr NewNode = ness_make_ptr<ParticlesNode>(this->m_renderer, EstimatedSize);
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