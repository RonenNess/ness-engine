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
#include "../exceptions/exceptions.h"
#include "animators_queue.h"
#include <algorithm>

namespace Ness
{

	namespace Animators
	{
		// empty deleter that does nothing. 
		// this is a way to create animator shared pointer that don't delete the object itself, so we'll be able to register anumator with a regular pointer
		static void EmptyAnimatorDeleter(Animators::AnimatorAPI* anim)
		{
		}

		// register an animator by regular pointer
		void AnimatorsQueue::__register_animator_unsafe(Animators::AnimatorAPI* animator)
		{
			register_animator(Animators::AnimatorPtr(animator, EmptyAnimatorDeleter));
		}

		// remove an animator by regular pointer
		void AnimatorsQueue::__remove_animator_unsafe(Animators::AnimatorAPI* animator)
		{
			remove_animator(Animators::AnimatorPtr(animator, EmptyAnimatorDeleter));
		}

		// register an animator
		void AnimatorsQueue::register_animator(const Animators::AnimatorPtr& animator)
		{
			m_animators.push_back(animator);
			animator->__change_animator_queue(this);
		}

		// remove an animator
		void AnimatorsQueue::remove_animator(const Animators::AnimatorPtr& animator)
		{
			if (animator->__get_animator_queue() != this)
			{
				throw IllegalAction("Cannot remove animator, animator is not even in this queue!");
			}
			m_animators.erase(std::remove(m_animators.begin(), m_animators.end(), animator), m_animators.end());
			animator->__change_animator_queue(nullptr);
		}

		// remove all dead animators from list
		bool remove_dead_animators(const Animators::AnimatorPtr& animator)
		{
			return animator->__should_be_removed();
		}

		// run all animators
		void AnimatorsQueue::do_animations()
		{
			// loop and animate all animators
			bool gotAnimatorsToRemove = false;
			for (unsigned int i = 0; i < m_animators.size(); ++i)
			{
				Animators::AnimatorPtr& curr = m_animators[i];
				if (curr->is_animation_paused())
					continue;

				curr->do_animation(m_renderer);
				gotAnimatorsToRemove |= curr->__should_be_removed();
			}

			// remove all 'dead' animators
			if (gotAnimatorsToRemove)
			{
				m_animators.erase(std::remove_if(m_animators.begin(), m_animators.end(), remove_dead_animators), m_animators.end());
			}
		}
	};
};