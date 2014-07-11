/**
* A node that can contain son nodes or entities
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../resources/managed_texture.h"
#include "renderable.h"
#include <vector>

namespace Ness
{
	// some needed predeclares
	class Sprite;
	class Canvas;
	class Node;
	class ZNode;
	class TileMap;
	class RectangleShape;

	// node and sprite pointers
	NESSENGINE_API typedef std::shared_ptr<Node>				NodePtr;
	NESSENGINE_API typedef std::shared_ptr<ZNode>				ZNodePtr;
	NESSENGINE_API typedef std::shared_ptr<Sprite>				SpritePtr;
	NESSENGINE_API typedef std::shared_ptr<Canvas>				CanvasPtr;
	NESSENGINE_API typedef std::shared_ptr<RectangleShape>		RectangleShapePtr;
	NESSENGINE_API typedef std::shared_ptr<TileMap>				TileMapPtr;

	// a node can contain other nodes or renderable objects
	class Node : public Renderable, public RenderableParent
	{
	protected:
		std::vector<NodePtr>					m_nodes;						// son nodes
		std::vector<RenderablePtr>				m_entities;						// son sprites
		SRenderTransformations					m_absolute_trans;				// cache of last absolute transformations
		bool									m_need_trans_update;			// do we need to update the cached transformations (called if parent changed)
		ManagedResources::ManagedTexturePtr		m_render_target;				// if not null, will render to this target instead of to the screen

	public:
		NESSENGINE_API Node(Renderer* renderer, RenderableParent* parent = nullptr) : 
			Renderable(renderer, parent), m_need_trans_update(true) {}

		// add/remove sub 
		NESSENGINE_API virtual void add(const NodePtr& object);
		NESSENGINE_API virtual void add(const RenderablePtr& object);
		NESSENGINE_API virtual void remove(const NodePtr& object);
		NESSENGINE_API virtual void remove(const RenderablePtr& object);

		// called whenever transformations are updated
		NESSENGINE_API virtual void transformations_update();

		// get absolute transformations for this renderable node
		NESSENGINE_API virtual const SRenderTransformations& get_absolute_transformations();

		// create and return a son entities
		NESSENGINE_API virtual NodePtr create_node();
		NESSENGINE_API virtual NodePtr create_znode();
		NESSENGINE_API virtual RectangleShapePtr create_rectangle();
		NESSENGINE_API virtual SpritePtr create_sprite(const std::string& textureName);
		NESSENGINE_API virtual CanvasPtr create_canvas(const std::string& textureName, const Sizei& size = Sizei::ZERO);
		NESSENGINE_API virtual TileMapPtr create_tilemap(const std::string& spriteFile, Sizei mapSize, Size singleTileSize = Size(36, 36));

		// check if this node is really visible by checking all its sub nodes and sprites (until hitting yes)
		NESSENGINE_API virtual bool is_really_visible(const CameraPtr& camera = NullCamera);

		// set rendering target for this node
		NESSENGINE_API void set_render_target(ManagedResources::ManagedTexturePtr NewTarget) {m_render_target = NewTarget;}
		NESSENGINE_API void remove_render_target() {m_render_target.reset();}

		// render this node with camera
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera);

	};
};