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
		// sprite from spritesheet animation
		class AnimatorSprite : public AnimatorAPI
		{
		private:
			Sizei			m_spritesheet_total_steps;		// total steps (x and y) of the sprite sheet
			float			m_speed;						// animation speed
			SpritePtr		m_sprite;						// the sprite to animate
			float			m_currStep;						// current step
			unsigned int	m_starting;						// starting step
			bool			m_repeat;						// should the animator start again when finish animation cycle (if not, remove animator)
			unsigned int	m_count;						// how many steps in animation

		public:
			// if fadeIn = true, will increase opacity of object and remove animator once reach 1.0
			// if false, will reduce opacity until 0.0f
			AnimatorSprite(const Sizei& spritesheet_total_steps, unsigned int startingStep, unsigned int count, float AnimationSpeed = 1.0f, bool repeat = false) 
				: m_spritesheet_total_steps(spritesheet_total_steps), m_speed(AnimationSpeed), m_starting(startingStep), m_repeat(repeat), m_count(count)
			{
				m_currStep = (float)startingStep;
			}

			NESSENGINE_API virtual void set_target(RenderablePtr& object)
			{
				m_sprite = std::dynamic_pointer_cast<Sprite>(object);
				if (!m_sprite)
				{
					throw IllegalAction("Must provide a sprite to a sprite animator!");
				}
			}

			NESSENGINE_API virtual RenderablePtr get_target()
			{
				return m_sprite;
			}

			NESSENGINE_API virtual void animate(Renderer* renderer)
			{
				Pointi currStep;
				currStep.x = (int)m_currStep % m_spritesheet_total_steps.x;
				currStep.y = (int)(m_currStep / m_spritesheet_total_steps.x);
				m_sprite->set_source_from_sprite_sheet(currStep, m_spritesheet_total_steps);
				m_currStep += renderer->time_factor() * m_speed;

				if ((unsigned int)m_currStep > m_starting + m_count)
				{
					if (m_repeat)
					{
						m_currStep = (float)m_starting;
					}
					else
					{
						this->destroy();
					}
				}
			}
		};

	};
};