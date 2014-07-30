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
			// target - target sprite to animate
			// spritesheet_total_steps - how many steps the spritesheet animation has (x & y)
			// startingStep - counting from top-left corner and moving right, the position of the starting step for the animation (zero based)
			// stepsCount - how many steps this animation has (including first step)
			// animationSpeed - how fast to play this animation
			// endAction - what to do when animation ends cycle
			AnimatorSprite(const SpritePtr& target, const Sizei& spritesheet_total_steps, unsigned int startingStep, 
				unsigned int stepsCount, float AnimationSpeed = 1.0f, ESpriteAnimatorEnd endAction = SPRITE_ANIM_END_DO_NOTHING) : 
				m_spritesheet_total_steps(spritesheet_total_steps), 
				m_sprite(target),
				m_end_action(endAction)
			{
				reset(startingStep, stepsCount, AnimationSpeed);
			}

			// target - target sprite to animate
			// spritesheet_total_steps - how many steps the spritesheet animation has (x & y)
			// startingStep - x and y of the first step of the animation to play
			// stepsCount - how many steps this animation has (including first step)
			// animationSpeed - how fast to play this animation
			// endAction - what to do when animation ends cycle
			AnimatorSprite(const SpritePtr& target, const Sizei& spritesheet_total_steps, const Sizei& startingStep, 
				unsigned int stepsCount, float AnimationSpeed = 1.0f, ESpriteAnimatorEnd endAction = SPRITE_ANIM_END_DO_NOTHING) : 
				m_spritesheet_total_steps(spritesheet_total_steps), 
				m_sprite(target),
				m_end_action(endAction)
			{
				reset(startingStep, stepsCount, AnimationSpeed);
			}

			// return animation starting step
			NESSENGINE_API inline unsigned int get_starting_step() {return m_starting;}
			NESSENGINE_API inline Ness::Pointi get_starting_step_point() {return Ness::Pointi(m_starting % m_spritesheet_total_steps.x, m_starting / m_spritesheet_total_steps.x);}

			// reset animation
			NESSENGINE_API inline void reset()
			{
				m_currStep = (float)m_starting;
			}

			// change animation + reset
			NESSENGINE_API inline void reset(unsigned int startingStep, unsigned int stepsCount, float AnimationSpeed = 1.0f)
			{
				m_count = stepsCount;
				m_starting = startingStep;
				m_speed = AnimationSpeed;
				m_currStep = (float)m_starting;
			}

			// change animation + reset
			NESSENGINE_API inline void reset(const Pointi& startingStep, unsigned int stepsCount, float AnimationSpeed = 1.0f)
			{
				m_count = stepsCount;
				m_starting = startingStep.x + (startingStep.y * m_spritesheet_total_steps.x);
				m_speed = AnimationSpeed;
				m_currStep = (float)m_starting;
			}

			NESSENGINE_API virtual void do_animation(Renderer* renderer)
			{
				Pointi currStep;
				currStep.x = (int)m_currStep % m_spritesheet_total_steps.x;
				currStep.y = (int)(m_currStep / m_spritesheet_total_steps.x);
				m_sprite->set_source_from_sprite_sheet(currStep, m_spritesheet_total_steps);
				m_currStep += renderer->time_factor() * m_speed;

				if ((unsigned int)m_currStep >= m_starting + m_count)
				{
					switch (m_end_action)
					{
					case SPRITE_ANIM_END_REPEAT:
						m_currStep = (float)m_starting;
						break;
			
					case SPRITE_ANIM_END_REMOVE_SPRITE:
						m_sprite->parent()->remove(m_sprite);
					case SPRITE_ANIM_END_DO_NOTHING:
						this->remove_from_animation_queue();
						break;
					}
				}
			}
		};

		// define the animator pointer
		NESSENGINE_API typedef SharedPtr<AnimatorSprite> AnimatorSpritePtr;

	};
};