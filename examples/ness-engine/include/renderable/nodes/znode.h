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
* A scene node that takes z-order into consideration.
* this node will arrange entities and nodes automatically based on their z-index.
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "node.h"

namespace Ness
{
	/**
	* A special node that re-order the entities in realtime based on their z-value, creating a z-ordering effect.
	*/
	class ZNode : public Node
	{
	private:
		bool	m_break_groups;				// should we break son nodes when z-ordering or treat them as a single entity?
		float	m_update_list_intervals;	// time in miliseconds between z-ordering refresh
		float	m_time_until_next_zorder;	// time left until next time we need to z-order

	public:
		// create the znode
		NESSENGINE_API ZNode(Renderer* renderer) : Node(renderer), m_break_groups(false), m_update_list_intervals(0.05f), m_time_until_next_zorder(0.0f) {}

		// this determine how often, in miliseconds, should this z-node update its rendering list,
		// i.e. recalculate all visible entities and reorder them.
		// a good balance would be quick enough not to notice z-ordering delays or object appearance delay,
		// but still not ordering every frame.
		NESSENGINE_API inline void set_reorder_interval(float interval) {m_update_list_intervals = interval;}

		// set if this znode should break groups or not
		// if break groups is true, this znode will take ALL entities from all son nodes and arrange them based on zorder.
		// if break groups is false, the znode will arrange son nodes as son entities, meaning every son node shares the same z-order.
		// NOTE: if you want a node that never will be broken, set it's flag with: node->set_flag(Ness::RNF_NEVER_BREAK);
		NESSENGINE_API inline void set_break_groups(bool BreakGroups) {m_break_groups = BreakGroups;}

		// render everything, with z order!
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera);
	};

	// scene pointer
	NESSENGINE_API typedef SharedPtr<ZNode> ZNodePtr;
};