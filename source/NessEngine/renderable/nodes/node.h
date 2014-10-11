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

/**
* A node that can contain son nodes or entities
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "basic_node.h"

namespace Ness
{
	// some needed predeclares
	class Sprite;
	class Canvas;
	class Text;
	class MultiText;
	class Node;
	class ZNode;
	class StaticNode;
	class TileMap;
	class NodesMap;
	class RectangleShape;
	class NodeAPI;
	class LightNode;
	class ParticlesNode;

	// node and sprite pointers
	NESSENGINE_API typedef SharedPtr<Node>				NodePtr;
	NESSENGINE_API typedef SharedPtr<ZNode>				ZNodePtr;
	NESSENGINE_API typedef SharedPtr<StaticNode>		StaticNodePtr;
	NESSENGINE_API typedef SharedPtr<LightNode>			LightNodePtr;
	NESSENGINE_API typedef SharedPtr<Sprite>			SpritePtr;
	NESSENGINE_API typedef SharedPtr<Canvas>			CanvasPtr;
	NESSENGINE_API typedef SharedPtr<Text>				TextPtr;
	NESSENGINE_API typedef SharedPtr<MultiText>			MultiTextPtr;
	NESSENGINE_API typedef SharedPtr<RectangleShape>	RectangleShapePtr;
	NESSENGINE_API typedef SharedPtr<TileMap>			TileMapPtr;
	NESSENGINE_API typedef SharedPtr<NodesMap>			NodesMapPtr;
	NESSENGINE_API typedef SharedPtr<ParticlesNode>		ParticlesNodePtr;
	NESSENGINE_API typedef SharedPtr<NodeAPI>			NodeAPIPtr;

	// a scene node with functionality to create all type of built-in entities and nodes
	class Node : public BaseNode
	{
	public:
		NESSENGINE_API Node(Renderer* renderer) : 
			BaseNode(renderer) {}

		// create and return a son entities
		NESSENGINE_API virtual NodePtr create_node(bool add_immediatly=true);
		NESSENGINE_API virtual ZNodePtr create_znode(bool add_immediatly=true);
		NESSENGINE_API virtual StaticNodePtr create_static_node(bool add_immediatly=true);
		NESSENGINE_API virtual LightNodePtr create_light_node(bool add_immediatly=true);
		NESSENGINE_API virtual RectangleShapePtr create_rectangle(bool add_immediatly=true);
		NESSENGINE_API virtual SpritePtr create_sprite(const String& textureName, bool add_immediatly=true);
		NESSENGINE_API virtual ParticlesNodePtr create_particles_node(const Size& EstimatedSize, bool add_immediatly=true);
		NESSENGINE_API virtual CanvasPtr create_canvas(const String& textureName, const Sizei& size = Sizei::ZERO, bool add_immediatly=true);
		NESSENGINE_API virtual TileMapPtr create_tilemap(const String& spriteFile, const Sizei& mapSize, const Size& singleTileSize=Size(36, 36), const Size& tilesDistance=Size::ZERO, bool add_immediatly=true);
		NESSENGINE_API virtual NodesMapPtr create_nodesmap(const Sizei& mapSize, const Size& singleNodeSize=Size(36, 36), const Size& nodesDistance=Size::ZERO, bool add_immediatly=true);
		NESSENGINE_API virtual TextPtr create_text(const String& fontFile, const String& text, int fontSize = 12, bool add_immediatly=true);
		NESSENGINE_API virtual MultiTextPtr create_multitext(const String& fontFile, const String& text, int fontSize = 12, bool add_immediatly=true);

	};
};