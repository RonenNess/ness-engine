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
* Animator that fade objects in and out
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
		* this animator scales the target over time
		*/
		class AnimatorRotator : public TargetAnimatorAPI
		{
		private:
			float			m_speed;			// rotating speed and direction
			float			m_time_to_rotate;	// for how long we will rotate the object
			bool			m_infinite;			// does this animator runs forever?
			float			m_time_until;		// time until starting animation

		public:
			// target is the renderable to fade-in. make sure it supports blending!
			// ExtraRotationPerSecond is how much rotation will be added over a period of 1 second.
			// for example, ExtraScalePerSecond(1, 1) means the target will scale by +1 to x and +1 to y within 1 second
			// TimeToRotate is for how long to keep the rotator running (0.0f for infinite rotation)
			// timeUntilRotate is how long in seconds to wait before starting to rotate
			AnimatorRotator(const RenderablePtr& target, float ExtraRotationPerSecond, float TimeToRotate = 1.0f, float timeUntilRotate = 0.0f) 
				: TargetAnimatorAPI(target),  m_speed(ExtraRotationPerSecond), m_time_to_rotate(TimeToRotate), m_time_until(timeUntilRotate), m_infinite(TimeToRotate == 0.0f)
			{
			}


			NESSENGINE_API virtual void do_animation(Renderer* renderer)
			{
				if (m_time_until > 0.0f)
				{
					m_time_until -= renderer->time_factor();
					return;
				}

				m_target->add_rotation(m_speed * renderer->time_factor());

				if (m_infinite == false)
				{
					m_time_to_rotate -= renderer->time_factor();
					if (m_time_to_rotate <= 0)
					{
						this->remove_from_animation_queue();
					}
				}
			}
		};

		// fade-in animator pointer
		NESSENGINE_API typedef SharedPtr<AnimatorRotator> AnimatorRotatorPtr;
	};
};