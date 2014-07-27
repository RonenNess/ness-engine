/**
* An optimized scene node that renders everything on a matrix of canvases, and update them only when necessary.
* This is a great optimization for mostly static rendering that don't require z-ordering. for example, you can put an entire tilemap inside this node to optimize tilemap.
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "node.h"

namespace Ness
{
	class OptNode : public Node
	{
	private:
		bool m_update_automatically;

	public:
		// create the znode
		NESSENGINE_API OptNode(Renderer* renderer) : Node(renderer), m_update_automatically(true) {}

		// if true, will check every time it renders if need updates and update all the batches that were changed.
		// if false, will update batches only when you manually call 'update_batches()'.
		// use automatic updates if you know your optimized node might update once in a while, disable if your node is 100% static.
		// if your optimized node changes a lot, don't use an optimized node!
		inline void set_update_automatically(bool Enable) {m_update_automatically = Enable;}

		// update all the canvas batches
		NESSENGINE_API virtual void update_batches(bool OnlyVisible = false);

		// update all the currently visible batches based on camera
		NESSENGINE_API virtual void update_visible_batches(const CameraPtr& camera = NullCamera);

		// render everything, with z order!
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera);
	};

	// scene pointer
	NESSENGINE_API typedef NessSharedPtr<OptNode> OptNodePtr;
};