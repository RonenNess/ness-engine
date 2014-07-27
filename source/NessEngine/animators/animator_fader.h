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
		// fade in animator
		class AnimatorFaderIn : public AnimatorAPI
		{
		private:
			RenderablePtr	m_target;	// fader target
			float			m_speed;	// fading speed

		public:
			
			AnimatorFaderIn(float fadeSpeed = 1.0f) : m_speed(fadeSpeed) {}

			NESSENGINE_API virtual void set_target(RenderablePtr object)
			{
				m_target = object;
			}

			NESSENGINE_API virtual RenderablePtr get_target()
			{
				return m_target;
			}

			NESSENGINE_API virtual void animate(Renderer* renderer)
			{
				m_target->set_opacity(m_target->get_opacity() + renderer->time_factor() * m_speed);
				if (m_target->get_opacity() >= 1.0f)
				{
					m_target->set_opacity(1.0f);
					this->remove_from_animation_queue();
				}
			}
		};

		// define the animator pointer
		NESSENGINE_API typedef SharedPtr<AnimatorFaderIn> AnimatorFaderInPtr;

		// fade-out animator
		class AnimatorFaderOut : public AnimatorAPI
		{
		private:
			RenderablePtr	m_target;			// fader target
			float			m_speed;			// fading speed
			bool			m_remove_when_done;	// remove target when done?

		public:
			// if removeTargetWhenDone = true, will remove the target from its parent when opacity reach 0
			AnimatorFaderOut(const RenderablePtr& target, bool removeTargetWhenDone, float fadeSpeed = 1.0f) 
				: m_target(target), m_remove_when_done(removeTargetWhenDone), m_speed(fadeSpeed) {}


			NESSENGINE_API virtual void do_animation(Renderer* renderer)
			{

				m_target->set_opacity(m_target->get_opacity() - renderer->time_factor() * m_speed);
				if (m_target->get_opacity() <= 0.0f)
				{
					m_target->set_opacity(0.0f);
					this->remove_from_animation_queue();
					if (m_remove_when_done)
					{
						m_target->parent()->remove(m_target);
					}
				}
			}
		};

		// define the animator pointer
		NESSENGINE_API typedef SharedPtr<AnimatorFaderOut> AnimatorFaderOutPtr;

	};
};