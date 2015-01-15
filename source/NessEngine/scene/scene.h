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
* A scene in which we can put nodes and objects to render
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../exports.h"
#include "../exceptions/exceptions.h"
#include "../renderable/nodes/node.h"
#include "../basic_types/containers.h"
#include "viewport.h"
#include "camera/all_cameras.h"

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
		NESSENGINE_API virtual void transformations_update() {m_absolute_trans = m_transformations; Node::transformations_update();}

		// render on a viewport
		NESSENGINE_API virtual void render_on_viewport(const ViewportPtr& viewport, const CameraApiPtr& camera);

		// disable the possibility to add a scene as a son
		NESSENGINE_API virtual void __change_parent(NodeAPI* parent) {throw IllegalAction("cannot assign scene under another scene or node!");}
	};

	// scene pointer
	NESSENGINE_API typedef SharedPtr<Scene> ScenePtr;
};