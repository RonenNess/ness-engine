/**
* Animator that animates sprite from spritesheet
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "animator_api.h"
#include "../renderer/renderer.h"
#include "../renderable/entities/sprite.h"

namespace Ness
{
	namespace Animators
	{
		// possible actions when completing the animation cycle of an AnimatorSprite
		enum ESpriteAnimatorEnd
		{
			SPRITE_ANIM_END_DO_NOTHING,				// will leave the sprite and remove the animator
			SPRITE_ANIM_END_REPEAT,					// will replay the animation
			SPRITE_ANIM_END_REMOVE_SPRITE,			// will remove animator + sprite

		};

		// sprite from spritesheet animation
		class AnimatorSprite : public AnimatorAPI
		{
		private:
			Sizei				m_spritesheet_total_steps;		// total steps (x and y) of the sprite sheet
			float				m_speed;						// animation speed
			SpritePtr			m_sprite;						// the sprite to animate
			float				m_currStep;						// current step
			unsigned int		m_starting;						// starting step
			ESpriteAnimatorEnd	m_end_action;					// what to do when animation cycle ends
			unsigned int		m_count;						// how many steps in animation

		public:
			// if fadeIn = true, will increase opacity of object and remove animator once reach 1.0
			// if false, will reduce opacity until 0.0f
			AnimatorSprite(const SpritePtr& target, const Sizei& spritesheet_total_steps, unsigned int startingStep, 
				unsigned int stepsCount, float AnimationSpeed = 1.0f, ESpriteAnimatorEnd endAction = SPRITE_ANIM_END_DO_NOTHING) 
				: m_spritesheet_total_steps(spritesheet_total_steps), 
				m_speed(AnimationSpeed), 
				m_sprite(target),
				m_starting(startingStep), 
				m_end_action(endAction), 
				m_count(stepsCount)
			{
				m_currStep = (float)startingStep;
			}

			NESSENGINE_API virtual void do_animation(Renderer* renderer)
			{
				Pointi currStep;
				currStep.x = (int)m_currStep % m_spritesheet_total_steps.x;
				currStep.y = (int)(m_currStep / m_spritesheet_total_steps.x);
				m_sprite->set_source_from_sprite_sheet(currStep, m_spritesheet_total_steps);
				m_currStep += renderer->time_factor() * m_speed;

				if ((unsigned int)m_currStep > m_starting + m_count)
				{
					switch (m_end_action)
					{
					case SPRITE_ANIM_END_REPEAT:
						m_currStep = (float)m_starting;
						break;
			
					case SPRITE_ANIM_END_REMOVE_SPRITE:
						m_sprite->parent()->remove(m_sprite);
					case SPRITE_ANIM_END_DO_NOTHING:
						this->remove_animation();
						break;
					}
				}
			}
		};

	};
};