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
* Animator that shift entity color over time
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
		* this animator change the color of the entity from color A to color B over a const period of time.
		*/
		class AnimatorColorShifter : public TargetAnimatorAPI
		{
		private:
			Color			m_color_a;			// starting color
			Color			m_color_b;			// ending color
			float			m_speed;			// shifting color speed
			float			m_complete;			// complete shifting percent (1.0f is fully color B)
			float			m_time_until;		// time until starting animation
			bool			m_remove_when_done;	// remove object when done shifting color

		public:
			// target is the renderable to fade-in. make sure it supports blending!
			// ColorA is the starting color
			// ColorB is the result color
			// timeToShift is how long, in seconds, it will take to change from color A to color B
			// timeUntilBegin is time to wait, in seconds, until starting the color shifting
			// removeWhenDone if true, will remove the target object once transition is complete
			AnimatorColorShifter(const RenderablePtr& target, Color ColorA, Color ColorB, float timeToShift = 1.0f, float timeUntilBegin = 0.0f, bool removeWhenDone = false) 
				: TargetAnimatorAPI(target), m_color_a(ColorA), m_color_b(ColorB),  m_speed(1.0f / timeToShift), 
				m_complete(0.0f), m_time_until(timeUntilBegin), m_remove_when_done(removeWhenDone)
			{
				// set target to starting color
				target->set_color(ColorA);
			}


			NESSENGINE_API virtual void do_animation(Renderer* renderer)
			{
				// wait until starting animation
				if (m_time_until > 0.0f)
				{
					m_time_until -= renderer->time_factor();
					return;
				}

				// check if finished animation (remove self from animating queue and check if need to remove target)
				if (m_complete >= 1.0f)
				{
					if (m_remove_when_done)
					{
						m_target->parent()->remove(m_target);
					}
					this->remove_from_animation_queue();
					return;
				}

				// increase complete percentage
				m_complete += renderer->time_factor() * m_speed;
				if (m_complete > 1.0f) m_complete = 1.0f;

				// set color
				m_target->set_color((m_color_a * (1.0f - m_complete)) + (m_color_b * m_complete));
			}
		};

		// color shifter animator pointer
		NESSENGINE_API typedef SharedPtr<AnimatorColorShifter> AnimatorColorShifterPtr;
	};
};