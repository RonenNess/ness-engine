/**
* A scene in which we can put nodes and objects to render
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../exceptions/exceptions.h"
#include "../renderable/node.h"
#include "../renderable/sprite.h"
#include "../renderable/tile_map.h"
#include "../renderable/canvas.h"
#include "camera.h"
#include <vector>

namespace Ness
{
	class Scene : public Node
	{
	public:
		Scene(Renderer* renderer) : Node(renderer, nullptr) {}

		// scene should never have parent, so it's easy - absolute transformation is self transformation
		virtual const SRenderTransformations& get_absolute_transformations() {return m_absolute_trans;}

		// called whenever transformations are updated
		virtual void transformations_update() {m_absolute_trans = m_transformations;}

		// disable the possibility to add a scene as a son
		virtual void __change_parent(RenderableParent* parent) {throw IllegalAction("cannot assign scene under another scene or node!");}
	};

	// scene pointer
	typedef std::shared_ptr<Scene> ScenePtr;
};