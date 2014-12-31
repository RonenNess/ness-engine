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
* Animator that moves objects
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "animator_api.h"
#include "../renderer/renderer.h"

namespace Ness
{
	namespace Animators
	{

		/**
		* this animator moves the target over time
		*/
		class AnimatorMover : public TargetAnimatorAPI
		{
		private:
			Point		m_speed;			// moving speed and direction
			float			m_time_to_move;		// for how long we will move the object
			bool			m_infinite;			// does this animator runs forever?
			float			m_time_until;		// time until starting animation

		public:
			// target is the renderable to move
			// MovingSpeed is the speed and direction to move the entity
			// TimeToMove is for how long to keep the mover running (0.0f for infinite movement)
			// timeUntilMove is how long in seconds to wait before starting to move
			AnimatorMover(const RenderablePtr& target, Point MovingSpeed, float TimeToMove = 1.0f, float timeUntilMove = 0.0f) 
				: TargetAnimatorAPI(target),  m_speed(MovingSpeed), m_time_to_move(TimeToMove), m_time_until(timeUntilMove), m_infinite(TimeToMove == 0.0f)
			{
			}


			NESSENGINE_API virtual void do_animation(Renderer* renderer)
			{
				if (m_time_until > 0.0f)
				{
					m_time_until -= renderer->time_factor();
					return;
				}

				m_target->add_position(m_speed * renderer->time_factor());

				if (m_infinite == false)
				{
					m_time_to_move -= renderer->time_factor();
					if (m_time_to_move <= 0)
					{
						this->remove_from_animation_queue();
					}
				}
			}
		};

		// fade-in animator pointer
		NESSENGINE_API typedef SharedPtr<AnimatorMover> AnimatorMoverPtr;
	};
};