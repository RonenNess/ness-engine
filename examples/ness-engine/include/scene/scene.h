/**
* A scene in which we can put nodes and objects to render
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../exports.h"
#include "../exceptions/exceptions.h"
#include "../renderable/nodes/node.h"
#include "../basic_types/containers.h"
#include "camera.h"

namespace Ness
{
	/**
	* a scene is just the root node, you first create a scene and from that you can create entities and other son nodes.
	* you create a scene via the renderer class.
	*/
	class Scene : public Node
	{
	public:
		NESSENGINE_API Scene(Renderer* renderer) : Node(renderer) {}

		// scene should never have parent, so it's easy - absolute transformation is self transformation
		NESSENGINE_API virtual const SRenderTransformations& get_absolute_transformations() {return m_absolute_trans;}

		// called whenever transformations are updated
		NESSENGINE_API virtual void transformations_update() {m_absolute_trans = m_transformations;}

		// disable the possibility to add a scene as a son
		NESSENGINE_API virtual void __change_parent(NodeAPI* parent) {throw IllegalAction("cannot assign scene under another scene or node!");}
	};

	// scene pointer
	NESSENGINE_API typedef SharedPtr<Scene> ScenePtr;
};