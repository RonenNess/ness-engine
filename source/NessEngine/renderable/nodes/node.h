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
	class TileMap;
	class RectangleShape;
	class NodeAPI;
	class LightNode;

	// node and sprite pointers
	NESSENGINE_API typedef SharedPtr<Node>				NodePtr;
	NESSENGINE_API typedef SharedPtr<ZNode>				ZNodePtr;
	NESSENGINE_API typedef SharedPtr<LightNode>			LightNodePtr;
	NESSENGINE_API typedef SharedPtr<Sprite>			SpritePtr;
	NESSENGINE_API typedef SharedPtr<Canvas>			CanvasPtr;
	NESSENGINE_API typedef SharedPtr<Text>				TextPtr;
	NESSENGINE_API typedef SharedPtr<MultiText>			MultiTextPtr;
	NESSENGINE_API typedef SharedPtr<RectangleShape>	RectangleShapePtr;
	NESSENGINE_API typedef SharedPtr<TileMap>			TileMapPtr;
	NESSENGINE_API typedef SharedPtr<NodeAPI>			NodeAPIPtr;

	// a scene node that can create all type of built-in entities and nodes
	class Node : public BaseNode
	{
	public:
		NESSENGINE_API Node(Renderer* renderer) : 
			BaseNode(renderer) {}

		// create and return a son entities
		NESSENGINE_API virtual NodePtr create_node(bool add_immediatly=true);
		NESSENGINE_API virtual ZNodePtr create_znode(bool add_immediatly=true);
		NESSENGINE_API virtual LightNodePtr create_light_node(bool add_immediatly=true);
		NESSENGINE_API virtual RectangleShapePtr create_rectangle(bool add_immediatly=true);
		NESSENGINE_API virtual SpritePtr create_sprite(const String& textureName, bool add_immediatly=true);
		NESSENGINE_API virtual CanvasPtr create_canvas(const String& textureName, const Sizei& size = Sizei::ZERO, bool add_immediatly=true);
		NESSENGINE_API virtual TileMapPtr create_tilemap(const String& spriteFile, Sizei mapSize, Size singleTileSize = Size(36, 36), bool add_immediatly=true);
		NESSENGINE_API virtual TextPtr create_text(const String& fontFile, const String& text, int fontSize = 12, bool add_immediatly=true);
		NESSENGINE_API virtual MultiTextPtr create_multitext(const String& fontFile, const String& text, int fontSize = 12, bool add_immediatly=true);

	};
};