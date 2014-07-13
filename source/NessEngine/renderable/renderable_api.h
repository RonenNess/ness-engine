/**
* the API for any renderable object in the engine (both node and entity)
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../exports.h"
#include "../scene/camera.h"
#include "transformable_api.h"
#include "../exceptions/exceptions.h"

namespace Ness
{
	// predeclare required stuff
	class NodeAPI;
	class Renderer;

	// the API of a renderable object
	class RenderableAPI: public Transformable
	{
	protected:
		Renderer*				m_renderer;						// pointer to the renderer
		NodeAPI*				m_parent;						// parent node
		bool					m_visible;						// should this renderable be displayed or not

	public:
		NESSENGINE_API RenderableAPI(Renderer* renderer, NodeAPI* parent = nullptr) : 
		  m_renderer(renderer), m_parent(parent), m_visible(true) {}

		// enable/disable rendering of this object
		NESSENGINE_API inline void set_visible(bool Visible) {m_visible = Visible;}
		NESSENGINE_API inline bool is_visible() const {return m_visible;}

		// is this renderable object actually visible and inside screen?
		NESSENGINE_API virtual bool is_really_visible(const CameraPtr& camera = NullCamera) = 0;

		// render this object
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera) = 0;

		// change the parent of this object
		virtual void __change_parent(NodeAPI* parent) 
		{
			if (m_parent && parent && m_parent != parent) throw IllegalAction("Cannot change object parent without removing from previous parent first!");
			m_parent = parent;
		}

		// get parent of this object
		NESSENGINE_API inline NodeAPI* parent() const {return m_parent;}

		// get renderer of this object
		NESSENGINE_API inline Renderer* renderer() const {return m_renderer;}

	};

	// renderable object pointer
	NESSENGINE_API typedef std::shared_ptr<RenderableAPI> RenderablePtr;
};