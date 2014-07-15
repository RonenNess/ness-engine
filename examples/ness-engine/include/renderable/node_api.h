/**
* the API for a scene node object
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../basic_types/containers.h"
#include "renderable_api.h"
#include "transformations.h"

namespace Ness
{

	// the API of a scene node class
	class NodeAPI : public RenderableAPI
	{
	public:

		NESSENGINE_API NodeAPI(Renderer* renderer, NodeAPI* parent = nullptr) : 
		  RenderableAPI(renderer, parent) {}

		// is this node actually visible and inside screen?
		NESSENGINE_API virtual bool is_really_visible(const CameraPtr& camera = NullCamera) = 0;

		// get absolute transformations of this node
		virtual const SRenderTransformations& get_absolute_transformations() = 0;

		// add/remove entities from this node
		NESSENGINE_API virtual void add(const RenderablePtr& object) = 0;
		NESSENGINE_API virtual void remove(const RenderablePtr& object) = 0;

		// render this scene node (all nodes must also be renderable)
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera) = 0;

		// get a list with ALL the son entities that are currently visible in screen
		NESSENGINE_API virtual void __get_visible_entities(Vector<RenderableAPI*>& out_list, const CameraPtr& camera = NullCamera) = 0;
	};

	// renderable parent pointer
	NESSENGINE_API typedef NessSharedPtr<NodeAPI> NodeAPIPtr;
};