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

	// all ness-engine flags you can use with renderable node flags
	enum ERenderableNodeFlags
	{
		RNF_NEVER_BREAK = 0,		// never break this node when z-ordering inside a z-node
	};

	// all ness-engine flags you can use with renderable entity flags
	enum ERenderableEntityFlags
	{
	};

	// the API of any renderable object (entity or node)
	class RenderableAPI: public Transformable
	{
	protected:
		Renderer*				m_renderer;						// pointer to the renderer
		NodeAPI*				m_parent;						// parent node
		bool					m_visible;						// should this renderable be displayed or not
		int						m_flags;						// flags you can set on this renderable for any purpose
		void*					m_user_data;					// optional user data you can attach to this object
		String					m_name;							// optional name to assign to this renderable

	public:
		NESSENGINE_API RenderableAPI(Renderer* renderer) : 
		  m_renderer(renderer), m_parent(nullptr), m_visible(true), m_flags(0), m_user_data(nullptr) {}

		// attached customized user data to this object
		inline void set_user_data(void* user_data) {m_user_data = user_data;}
		const void* get_user_data() const {return m_user_data;}
		void* get_user_data() {return m_user_data;}

		// enable/disable rendering of this object
		NESSENGINE_API inline void set_visible(bool Visible) {m_visible = Visible;}
		NESSENGINE_API inline bool is_visible() const {return m_visible;}

		// set/get this entity name
		NESSENGINE_API inline void set_name(const String& name) {m_name = name;}
		NESSENGINE_API inline const String& get_name() const {return m_name;}

		// set/get flags
		NESSENGINE_API inline int	get_flags() const {return m_flags;}
		NESSENGINE_API inline void	set_flags(int flags) {m_flags = flags;}
		NESSENGINE_API inline bool	get_flag(int flag) const {return ((m_flags >> flag) & 0x1);}
		NESSENGINE_API inline void	set_flag(int flag) {(m_flags |= (0x1 << flag));}
		NESSENGINE_API inline void	unset_flag(int flag) {(m_flags &= ~(0x1 << flag));}

		// is it node or entity?
		NESSENGINE_API virtual bool is_node() const = 0;
		NESSENGINE_API virtual bool is_entity() const = 0;

		// is this renderable object actually visible and inside screen?
		NESSENGINE_API virtual bool is_really_visible(const CameraPtr& camera = NullCamera) = 0;

		// render this object
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera) = 0;

		// remove this entity from parent
		NESSENGINE_API virtual void remove_from_parent();

		// change the parent of this object
		NESSENGINE_API virtual void __change_parent(NodeAPI* parent);

		// get parent of this object
		NESSENGINE_API inline NodeAPI* parent() const {return m_parent;}

		// get renderer of this object
		NESSENGINE_API inline Renderer* renderer() const {return m_renderer;}

	};

	// renderable object pointer
	NESSENGINE_API typedef SharedPtr<RenderableAPI> RenderablePtr;
};