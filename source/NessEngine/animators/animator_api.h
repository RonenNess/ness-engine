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
* the API for animator object - object that attach to a renderable and "runs" on it every frame.
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../exports.h"
#include "../basic_types/pointers.h"

namespace Ness
{
	// predeclare renderer and renderer API
	class Renderer;
	class RenderableAPI;
	NESSENGINE_API typedef SharedPtr<RenderableAPI> RenderablePtr;

	namespace Animators
	{
		
		// predeclare animators queue class
		class AnimatorsQueue;

		/**
		* Animator class API.
		* to create an animator inherit from this class and implement do_animation(), which is the function that will be called
		* by the renderer every frame. to remove animator from animators queue call remove_from_animation_queue().
		*/
		class AnimatorAPI
		{
		private:
			bool m_animator_paused;
			bool m_animator_should_be_removed;

		protected:
			AnimatorsQueue* m_animator_queue_parent;

		public:
			NESSENGINE_API AnimatorAPI() : m_animator_paused(false), m_animator_should_be_removed(false), m_animator_queue_parent(nullptr) {}
			NESSENGINE_API ~AnimatorAPI();

			// pause/unpause animation without removing from animators queue
			NESSENGINE_API inline bool is_animation_paused() const {return m_animator_paused;}
			NESSENGINE_API inline void pause_animation(bool doPause) {m_animator_paused = doPause;}

			// called whenever registered to a new animators queue, to update the parent animator queue
			NESSENGINE_API void __change_animator_queue(AnimatorsQueue* NewQueue);
			NESSENGINE_API inline AnimatorsQueue* __get_animator_queue() {return m_animator_queue_parent;}

			// remove this animator from the renderer animators queue (will no longer run)
			NESSENGINE_API void remove_from_animation_queue() {m_animator_should_be_removed = true;}
			NESSENGINE_API inline bool __should_be_removed() const {return m_animator_should_be_removed;}

			// animate this object. when finish, if you want to remove animator call 'remove_from_animation_queue()'
			NESSENGINE_API virtual void do_animation(Renderer* renderer) = 0;
		};

		/**
		* Animator that operates on a given target.
		* these animators are used as external objects that can animate entities or nodes.
		*/
		class TargetAnimatorAPI : public AnimatorAPI
		{
		protected:
			RenderablePtr	m_target;

		public:
			NESSENGINE_API TargetAnimatorAPI(RenderablePtr	target) : m_target(target) {}
			NESSENGINE_API inline const RenderablePtr& get_target() const {return m_target;}
			NESSENGINE_API inline RenderablePtr& get_target() {return m_target;}
		};

		// pointer to animator classes
		NESSENGINE_API typedef SharedPtr<AnimatorAPI> AnimatorPtr;
		NESSENGINE_API typedef SharedPtr<TargetAnimatorAPI> TargetAnimatorPtr;

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