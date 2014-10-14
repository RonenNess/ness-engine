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
#include "animator_api.h"
#include "animators_queue.h"
#include "../exceptions/exceptions.h"

namespace Ness
{
	namespace Animators
	{
		AnimatorAPI::~AnimatorAPI()
		{
			if (m_animator_queue_parent)
			{
				m_animator_queue_parent->__remove_animator_unsafe(this);
			}
		}

		void AnimatorAPI::__change_animator_queue(AnimatorsQueue* NewQueue) 
		{
			if (NewQueue != nullptr)
			{
				if (m_animator_queue_parent && m_animator_queue_parent != NewQueue)
				{
					throw IllegalAction("This animator is already in an Animators queue! you must remove it from previous parent before adding to a new queue!");
				}
			}
			m_animator_queue_parent = NewQueue;
		}
	};
};