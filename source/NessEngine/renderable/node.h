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

	// node and sprite pointers
	typedef std::shared_ptr<Node>		NodePtr;
	typedef std::shared_ptr<ZNode>		ZNodePtr;
	typedef std::shared_ptr<Sprite>		SpritePtr;
	typedef std::shared_ptr<Canvas>		CanvasPtr;
	typedef std::shared_ptr<TileMap>	TileMapPtr;

	// a node can contain other nodes or renderable objects
	class Node : public Renderable, public RenderableParent
	{
	protected:
		std::vector<NodePtr>					m_nodes;						// son nodes
		std::vector<RenderablePtr>				m_sprites;						// son sprites
		SRenderTransformations					m_absolute_trans;				// cache of last absolute transformations
		bool									m_need_trans_update;			// do we need to update the cached transformations (called if parent changed)
		ManagedResources::ManagedTexturePtr		m_render_target;				// if not null, will render to this target instead of to the screen

	public:
		Node(Renderer* renderer, RenderableParent* parent = nullptr) : 
			Renderable(renderer, parent), m_need_trans_update(true) {}

		// add/remove sub 
		virtual void add(const NodePtr& object);
		virtual void add(const SpritePtr& object);
		virtual void add(const TileMapPtr& object);
		virtual void remove(const NodePtr& object);
		virtual void remove(const SpritePtr& object);
		virtual void remove(const TileMapPtr& object);

		// called whenever transformations are updated
		virtual void transformations_update();

		// get absolute transformations for this renderable node
		virtual const SRenderTransformations& get_absolute_transformations();

		// create and return a son node
		virtual NodePtr create_node();

		// create and return a son node with z-ordering
		virtual NodePtr create_znode();

		// create and return a sprite
		virtual SpritePtr create_sprite(const std::string& textureName);

		// check if this node is really visible by checking all its sub nodes and sprites (until hitting yes)
		virtual bool is_really_visible(const CameraPtr& camera = NullCamera);

		// create and return a canvas sprite (empty canvas you can render on)
		virtual CanvasPtr create_canvas(const std::string& textureName, const Sizei& size = Sizei::ZERO);

		// create and return a tileset
		virtual TileMapPtr create_tilemap(const std::string& spriteFile, Sizei mapSize, Size singleTileSize = Size(36, 36));

		// set rendering target for this node
		void set_render_target(ManagedResources::ManagedTexturePtr NewTarget) {m_render_target = NewTarget;}
		void remove_render_target() {m_render_target.reset();}

		// render this node with camera
		virtual void render(const CameraPtr& camera = NullCamera);

	};
};