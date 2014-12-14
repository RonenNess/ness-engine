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
	// predeclare the canvas entity
	class Canvas;
	NESSENGINE_API typedef SharedPtr<Canvas> CanvasPtr;

	// a basic scene node, without functionality to create entities (only adding and removing existing sons)
	// if you want to create your own node, it is recommended to inherit from this class
	class BaseNode : public NodeAPI
	{
	protected:
		Containers::Vector<RenderablePtr>		m_entities;						// son entities (nodes, sprites, etc..)
		SRenderTransformations					m_absolute_trans;				// cache of last absolute transformations
		bool									m_need_trans_update;			// do we need to update the cached transformations (called if parent changed)
		ManagedResources::ManagedTexturePtr		m_render_target;				// if not null, will render to this target instead of to the screen

	public:
		NESSENGINE_API BaseNode(Renderer* renderer) : 
			NodeAPI(renderer), m_need_trans_update(true) {}

		// add/remove sub entity or node
		NESSENGINE_API virtual void add(const RenderablePtr& object);
		NESSENGINE_API virtual void add_first(const RenderablePtr& object);
		NESSENGINE_API virtual void remove(const RenderablePtr& object);

		// clear all son entities and nodes from this node
		NESSENGINE_API void clear() {m_entities.clear();}

		// direct access to son entities (note: son entities are in vector so efficiecny is alright here)
		NESSENGINE_API virtual unsigned int get_sons_count() const {return (unsigned int)m_entities.size();}
		NESSENGINE_API virtual RenderablePtr get_son(unsigned int index) {return m_entities[index];}

		// get son from name. will return empty ptr if not found.
		// if there are multiple sons with the same name, will return the first one found.
		NESSENGINE_API virtual RenderablePtr get_son(const String& name);

		// called whenever transformations are updated
		NESSENGINE_API virtual void transformations_update();

		// get absolute transformations for this renderable node
		NESSENGINE_API virtual const SRenderTransformations& get_absolute_transformations();

		// return absolute position
		NESSENGINE_API inline Point get_absolute_position() {return get_absolute_transformations().position;}
		NESSENGINE_API inline Point get_absolute_position_const() const {return m_absolute_trans.position;}

		// get all visible son entities
		// camera is the camera object to check visibility
		// break_son_nodes if true (default) will break son nodes and take their entities. else, will
		// just put the whole nodes into the out list.
		NESSENGINE_API virtual void __get_visible_entities(Containers::Vector<RenderableAPI*>& out_list,
			const CameraPtr& camera = NullCamera, bool break_son_nodes = true);

		// get a list with ALL entities in node
		// if breakGroups is true, will break down son nodes as well. else, will add son nodes to the list as whole
		NESSENGINE_API virtual void __get_all_entities(Containers::Vector<RenderableAPI*>& out_list, bool breakGroups);

		// check if this node is really visible by checking all its sub nodes and sprites (until hitting yes)
		NESSENGINE_API virtual bool is_really_visible(const CameraPtr& camera = NullCamera);

		// set/remove rendering target for this node
		NESSENGINE_API void set_render_target(const ManagedResources::ManagedTexturePtr& NewTarget);
		NESSENGINE_API void set_render_target(const CanvasPtr& NewTarget);
		NESSENGINE_API void remove_render_target();

		// return if need transformations udpate
		NESSENGINE_API virtual bool need_transformations_update() {return m_need_trans_update;}

		// render this node with camera
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera);

	};

	NESSENGINE_API typedef SharedPtr<BaseNode> BaseNodePtr;
};