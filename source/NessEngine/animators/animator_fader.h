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

		// fade-out animator
		class AnimatorFaderIn : public AnimatorAPI
		{
		private:
			RenderablePtr	m_target;			// fader target
			float			m_speed;			// fading speed
			float			m_time_until;		// time until starting animation

		public:
			// target is the renderable to fade-in
			// fadeSpeed is the speed of the fadein animation
			// timeUntilFadeIn is time to wait before starting to fade-in (1.0f == one second, default is 0.0f - start immediatly)
			AnimatorFaderIn(const RenderablePtr& target, float fadeSpeed = 1.0f, float timeUntilFadeIn = 0.0f) 
				: m_target(target),  m_speed(fadeSpeed), m_time_until(timeUntilFadeIn) 
			{
				target->set_opacity(0.0f);
			}


			NESSENGINE_API virtual void do_animation(Renderer* renderer)
			{
				if (m_time_until > 0.0f)
				{
					m_time_until -= renderer->time_factor();
					return;
				}

				m_target->set_opacity(m_target->get_opacity() - renderer->time_factor() * m_speed);
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
			float			m_time_until;		// time until starting animation
			bool			m_remove_when_done;	// remove target when done?

		public:
			// if removeTargetWhenDone = true, will remove the target from its parent when opacity reach 0
			// fadeSpeed is the speed of the fadeout animation
			// timeUntilFadeOut is time to wait before starting to fade-out (1.0f == one second, default is 0.0f - start immediatly)
			AnimatorFaderOut(const RenderablePtr& target, bool removeTargetWhenDone = true, float fadeSpeed = 1.0f, float timeUntilFadeOut = 0.0f) 
				: m_target(target), m_remove_when_done(removeTargetWhenDone), m_speed(fadeSpeed), m_time_until(timeUntilFadeOut) {}


			NESSENGINE_API virtual void do_animation(Renderer* renderer)
			{
				if (m_time_until > 0.0f)
				{
					m_time_until -= renderer->time_factor();
					return;
				}

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