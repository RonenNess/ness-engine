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
	private:
		bool m_break_groups;

	public:
		// create the znode
		NESSENGINE_API ZNode(Renderer* renderer, NodeAPI* parent = nullptr) : Node(renderer, parent), m_break_groups(false) {}

		// set if this znode should break groups or not
		// if break groups is true, this znode will take ALL entities from all son nodes and arrange them based on zorder.
		// if break groups is false, the znode will arrange son nodes as son entities, meaning every son node shares the same z-order.
		inline void set_break_groups(bool BreakGroups) {m_break_groups = BreakGroups;}

		// render everything, with z order!
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera);
	};

	// scene pointer
	NESSENGINE_API typedef std::shared_ptr<ZNode> ZNodePtr;
};