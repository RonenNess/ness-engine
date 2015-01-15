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

	// define a list of renderables
	typedef Containers::Vector<SharedPtr<RenderableAPI> > RenderablesList;

	// define a list of entities
	class EntityAPI;
	typedef Containers::Vector<SharedPtr<EntityAPI> > EntitiesList;

	// the API of a node class (containing other nodes and entities)
	class NodeAPI : public RenderableAPI
	{
	private:

	public:

		NESSENGINE_API NodeAPI(Renderer* renderer) : 
		  RenderableAPI(renderer) {}

		// is this node actually visible and inside screen?
		NESSENGINE_API virtual bool is_really_visible(const CameraApiPtr& camera) = 0;

		// clear should remove all the sons of this node and clean it up nicely.
		NESSENGINE_API virtual void destroy() = 0;

		// get absolute transformations of this node
		virtual const SRenderTransformations& get_absolute_transformations() = 0;

		// add/remove entities from this node
		// add first adds to the begining of the list, i.e. to the absolute background.
		NESSENGINE_API virtual void add(const RenderablePtr& object) = 0;
		NESSENGINE_API virtual void add_first(const RenderablePtr& object) = 0;
		NESSENGINE_API virtual void remove(const RenderablePtr& object) = 0;

		// add/remove from regular pointer (this is so objects can add/remove themselves. it's memory-leakage unsafe)
		NESSENGINE_API virtual void __add_unsafe(RenderableAPI* object);
		NESSENGINE_API virtual void __add_first_unsafe(RenderableAPI* object);
		NESSENGINE_API virtual void __remove_unsafe(RenderableAPI* object);

		// direct access to son entities (note: son entities are in vector so efficiecny is alright here)
		NESSENGINE_API virtual unsigned int get_sons_count() const = 0;
		NESSENGINE_API virtual RenderablePtr get_son(unsigned int index) = 0;

		// is it node or entity?
		NESSENGINE_API virtual bool is_node() const {return true;}
		NESSENGINE_API virtual bool is_entity() const {return false;}

		// render this scene node (all nodes must also be renderable)
		NESSENGINE_API virtual void render(const CameraApiPtr& camera) = 0;

		// select son entities from position (note: entities only!)
		// out_list is the list to fill with the selected entities
		// pos is the position to pick entities from (will return all entities that "touch" that position)
		// recursive if true, will break and scan son nodes. if false, will ignore son nodes
		// note: this will return only entities that have the RNF_SELECTABLE flag enabled (default state)
		NESSENGINE_API virtual void select_entities_from_position(EntitiesList& out_list, const Pointf& pos, bool recursive) const = 0;

		// get a list with ALL the son entities that are currently visible in screen
		// camera is to check visibility (which objects are in screen)
		// break_son_nodes if true will break the son nodes as well, else, will just put
		// son nodes as whole. NOTE: the NEVER_BREAK flag should not be tested inside this function,
		// it should be checked and handled by the caller. when this function is called with break_son_nodes = true,
		// it should ALWAYS break son-nodes (except for the son-nodes that have never-break flag which we need to 
		// check here as the callers)
		NESSENGINE_API virtual void __get_visible_entities(RenderablesList& out_list,
			const CameraApiPtr& camera, bool break_son_nodes = true) = 0;

		// nodes cannot be static.
		NESSENGINE_API virtual bool is_static() const {return false;}

		// get a list with ALL entities in node
		// if breakGroups is true, will break down son nodes as well. else, will add son nodes to the list as whole
		NESSENGINE_API virtual void __get_all_entities(RenderablesList& out_list, bool breakGroups) = 0;
	};

	// renderable parent pointer
	NESSENGINE_API typedef SharedPtr<NodeAPI> NodeAPIPtr;
};