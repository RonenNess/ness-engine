/**
* A node that can contain son nodes or entities
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../../managed_resources/managed_texture.h"
#include "../node_api.h"
#include "../transformable_api.h"
#include "../../basic_types/containers.h"

namespace Ness
{
	// some needed predeclares
	class Sprite;
	class Canvas;
	class Text;
	class Node;
	class ZNode;
	class TileMap;
	class RectangleShape;
	class NodeAPI;

	// node and sprite pointers
	NESSENGINE_API typedef NessSharedPtr<Node>				NodePtr;
	NESSENGINE_API typedef NessSharedPtr<ZNode>				ZNodePtr;
	NESSENGINE_API typedef NessSharedPtr<Sprite>			SpritePtr;
	NESSENGINE_API typedef NessSharedPtr<Canvas>			CanvasPtr;
	NESSENGINE_API typedef NessSharedPtr<Text>				TextPtr;
	NESSENGINE_API typedef NessSharedPtr<RectangleShape>	RectangleShapePtr;
	NESSENGINE_API typedef NessSharedPtr<TileMap>			TileMapPtr;
	NESSENGINE_API typedef NessSharedPtr<NodeAPI>			NodeAPIPtr;

	// a node can contain other nodes or renderable objects
	class Node : public NodeAPI
	{
	protected:
		Vector<RenderablePtr>				m_entities;						// son entities (nodes, sprites, etc..)
		SRenderTransformations					m_absolute_trans;				// cache of last absolute transformations
		bool									m_need_trans_update;			// do we need to update the cached transformations (called if parent changed)
		ManagedResources::ManagedTexturePtr		m_render_target;				// if not null, will render to this target instead of to the screen

	public:
		NESSENGINE_API Node(Renderer* renderer, NodeAPI* parent = nullptr) : 
			NodeAPI(renderer, parent), m_need_trans_update(true) {}

		// add/remove sub 
		NESSENGINE_API virtual void add(const RenderablePtr& object);
		NESSENGINE_API virtual void remove(const RenderablePtr& object);

		// called whenever transformations are updated
		NESSENGINE_API virtual void transformations_update();

		// get absolute transformations for this renderable node
		NESSENGINE_API virtual const SRenderTransformations& get_absolute_transformations();

		// get all visible son entities
		NESSENGINE_API virtual void __get_visible_entities(Vector<RenderableAPI*>& out_list, const CameraPtr& camera = NullCamera);

		// create and return a son entities
		NESSENGINE_API virtual NodePtr create_node();
		NESSENGINE_API virtual ZNodePtr create_znode();
		NESSENGINE_API virtual RectangleShapePtr create_rectangle();
		NESSENGINE_API virtual SpritePtr create_sprite(const std::string& textureName);
		NESSENGINE_API virtual CanvasPtr create_canvas(const std::string& textureName, const Sizei& size = Sizei::ZERO);
		NESSENGINE_API virtual TileMapPtr create_tilemap(const std::string& spriteFile, Sizei mapSize, Size singleTileSize = Size(36, 36));
		NESSENGINE_API virtual TextPtr create_text(const std::string& fontFile, const std::string& text, int fontSize = 12);

		// check if this node is really visible by checking all its sub nodes and sprites (until hitting yes)
		NESSENGINE_API virtual bool is_really_visible(const CameraPtr& camera = NullCamera);

		// set rendering target for this node
		NESSENGINE_API void set_render_target(ManagedResources::ManagedTexturePtr NewTarget) {m_render_target = NewTarget;}
		NESSENGINE_API void remove_render_target() {m_render_target.reset();}

		// render this node with camera
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera);

	};
};