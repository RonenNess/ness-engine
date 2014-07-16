/**
* The very basics of a scene node.
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

	// a basic scene node, without functionality to create entities (only adding and removing)
	class BaseNode : public NodeAPI
	{
	protected:
		Vector<RenderablePtr>					m_entities;						// son entities (nodes, sprites, etc..)
		SRenderTransformations					m_absolute_trans;				// cache of last absolute transformations
		bool									m_need_trans_update;			// do we need to update the cached transformations (called if parent changed)
		ManagedResources::ManagedTexturePtr		m_render_target;				// if not null, will render to this target instead of to the screen

	public:
		NESSENGINE_API BaseNode(Renderer* renderer, NodeAPI* parent = nullptr) : 
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

		// check if this node is really visible by checking all its sub nodes and sprites (until hitting yes)
		NESSENGINE_API virtual bool is_really_visible(const CameraPtr& camera = NullCamera);

		// set rendering target for this node
		NESSENGINE_API void set_render_target(ManagedResources::ManagedTexturePtr NewTarget) {m_render_target = NewTarget;}
		NESSENGINE_API void remove_render_target() {m_render_target.reset();}

		// render this node with camera
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera);

	};
};