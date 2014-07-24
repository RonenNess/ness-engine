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

		// the animator class API
		class AnimatorAPI
		{
		private:
			bool m_animator_paused;
			bool m_animator_should_be_removed;

		public:
			AnimatorAPI() : m_animator_paused(false), m_animator_should_be_removed(false) {}

			// pause/unpause animation
			NESSENGINE_API inline bool is_animation_paused() const {return m_animator_paused;}
			NESSENGINE_API inline void pause_animation(bool doPause) {m_animator_paused = doPause;}

			// destroy this animator
			NESSENGINE_API void remove_from_animation_queue() {m_animator_should_be_removed = true;}
			inline bool __should_be_removed() const {return m_animator_should_be_removed;}

			// animate this object. note: to destroy the animator call 'destroy()', even from within the animator itself.
			NESSENGINE_API virtual void do_animation(Renderer* renderer) = 0;
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