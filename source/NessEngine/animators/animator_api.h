/**
* the API for animator object - object that attach to a renderable and "runs" on it every frame.
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../exports.h"
#include "../renderable/renderable_api.h"


namespace Ness
{
	namespace Animators
	{
		// the animator class API
		class AnimatorAPI
		{
		private:
			bool m_paused;
			bool m_should_be_removed;

		public:
			AnimatorAPI() : m_paused(false), m_should_be_removed(false) {}

			// pause/unpause animation
			NESSENGINE_API inline bool is_paused() const {return m_paused;}
			NESSENGINE_API inline void pause(bool doPause) {m_paused = doPause;}

			// destroy this animator
			NESSENGINE_API void destroy() {m_should_be_removed = true;}
			inline bool __should_be_removed() const {return m_should_be_removed;}
		
			// set the animated object or add it to a list if this animator support multiply objects
			NESSENGINE_API virtual void set_target(RenderablePtr object) = 0;

			// animate this object. note: to destroy the animator call 'destroy()', even from within the animator itself.
			NESSENGINE_API virtual void animate(Renderer* renderer) = 0;
		};

		// define the animator pointer
		NESSENGINE_API typedef std::shared_ptr<AnimatorAPI> AnimatorPtr;
	};
};