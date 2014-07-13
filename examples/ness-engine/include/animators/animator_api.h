/**
* the API for animator object - object that attach to a renderable and "runs" on it every frame.
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../exports.h"
#include "../renderable/renderable_api.h"

class AnimationsManager;

namespace Ness
{
	// the animator class API
	class AnimatorAPI
	{
	private:
		AnimationsManager* m_manager;
	public:
		
		// set the animated object or add it to a list if this animator support multiply objects
		NESSENGINE_API virtual void set_animated(RenderablePtr);

		// animate this object. return false when you want to remove this animator from the animations manager
		NESSENGINE_API virtual bool animate();
	};
};