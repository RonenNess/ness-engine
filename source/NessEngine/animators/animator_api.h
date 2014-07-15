/**
* the API for animator object - object that attach to a renderable and "runs" on it every frame.
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../exports.h"

namespace Ness
{
	class Renderer;
	class RenderableAPI;
	NESSENGINE_API typedef NessSharedPtr<RenderableAPI> RenderablePtr;

	namespace Animators
	{
		class AnimatorAPI;

		// callback to call when animation is finished
		NESSENGINE_API typedef void (*FinishAnimationCallback)(AnimatorAPI* animator);

		// the animator class API
		class AnimatorAPI
		{
		private:
			bool m_paused;
			bool m_should_be_removed;
			FinishAnimationCallback m_callback;

		public:
			AnimatorAPI() : m_paused(false), m_should_be_removed(false), m_callback(nullptr) {}

			// register callback to call when animation finish
			NESSENGINE_API inline void set_animation_end_callback(FinishAnimationCallback callback) {m_callback = callback;}

			// pause/unpause animation
			NESSENGINE_API inline bool is_paused() const {return m_paused;}
			NESSENGINE_API inline void pause(bool doPause) {m_paused = doPause;}

			// destroy this animator
			NESSENGINE_API void destroy() {m_should_be_removed = true; if (m_callback) m_callback(this);}
			inline bool __should_be_removed() const {return m_should_be_removed;}
		
			// set the animated object or add it to a list if this animator support multiply objects
			NESSENGINE_API virtual void set_target(RenderablePtr object) = 0;

			// animate this object. note: to destroy the animator call 'destroy()', even from within the animator itself.
			NESSENGINE_API virtual void animate(Renderer* renderer) = 0;
		};

		// define the animator pointer
		NESSENGINE_API typedef NessSharedPtr<AnimatorAPI> AnimatorPtr;


		// template function to create an animator pointer (requires C++11 features)
		#if __cplusplus <= 199711L
		#else
				template <class T, typename... _Args>
				NESSENGINE_API AnimatorPtr make_animator(_Args && ... args)
				{
					return AnimatorPtr(new T(args));
				}
		#endif
		
	};
};