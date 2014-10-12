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
		class AnimatorScaler : public TargetAnimatorAPI
		{
		private:
			Point			m_speed;			// scaling speed
			float			m_time_to_scale;	// for how long we will scale the object
			float			m_time_until;		// time until starting animation

		public:
			// target is the renderable to fade-in. make sure it supports blending!
			// ExtraScalePerSecond is how much scale will be increased over a period of 1 second.
			// for example, ExtraScalePerSecond(1, 1) means the target will scale by +1 to x and +1 to y within 1 second
			// TimeToScale is for how long to keep the scaler running
			// timeUntilScale is how long in seconds to wait before starting to scale
			AnimatorScaler(const RenderablePtr& target, const Point& ExtraScalePerSecond, float TimeToScale = 1.0f, float timeUntilScale = 0.0f) 
				: TargetAnimatorAPI(target),  m_speed(ExtraScalePerSecond), m_time_to_scale(TimeToScale), m_time_until(timeUntilScale) 
			{
			}


			NESSENGINE_API virtual void do_animation(Renderer* renderer)
			{
				if (m_time_until > 0.0f)
				{
					m_time_until -= renderer->time_factor();
					return;
				}

				m_target->add_scale(m_speed * renderer->time_factor());
				m_time_to_scale -= renderer->time_factor();
				if (m_time_to_scale <= 0)
				{
					this->remove_from_animation_queue();
				}
			}
		};

		// fade-in animator pointer
		NESSENGINE_API typedef SharedPtr<AnimatorScaler> AnimatorScalerPtr;
	};
};