/**
* A scene node that takes z-order into consideration
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "node.h"

namespace Ness
{
	class ZNode : public Node
	{
	public:
		NESSENGINE_API ZNode(Renderer* renderer, RenderableParent* parent = nullptr) : Node(renderer, parent) {}

		// render everything, with z order!
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera);
	};

	// scene pointer
	NESSENGINE_API typedef std::shared_ptr<ZNode> ZNodePtr;
};