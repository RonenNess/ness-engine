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
* Animator that animates sprite from a spritesheet with advanced options.
* here you can actually define complex animation squences with different speed for every step and play animations by name.
* 
* Author: Ronen Ness
* Since: 11/1014
*/

#pragma once
#include "animator_api.h"
#include "../renderer/renderer.h"
#include "../renderable/entities/sprite.h"
#include <math.h>

namespace Ness
{
	namespace Animators
	{

		// represent a single step inside an animation sequence
		struct SpriteAnimationStep
		{
			Pointi position;		// the index of the animation step inside the spritesheet
			float duration;			// for how long to wait (in seconds) on this step before moving to the next step
		};

		// represent an entire animation sequence, composed of steps and optional texture
		struct SpriteAnimationSequence
		{
			String change_texture;								// optional, if not empty string will switch to the given texture
			Pointi texture_steps_count;							// how many steps there are in total in the texture spritesheet
			Containers::Vector<SpriteAnimationStep> steps;		// the steps of this animation sequence
		};

		// represent the container that holds all the animations of an animator spritesheet
		typedef Containers::UnorderedMap<String, SpriteAnimationSequence> AnimatorSpritesheetAnimations;

		// sprite from spritesheet animation
		class AnimatorSpriteCharacter : public TargetAnimatorAPI
		{
		private:
			SharedPtr<AnimatorSpritesheetAnimations>	m_animations;		// all the animation sequences (must be defined outside)
			float										m_speed;			// animations speed factor
			SpriteAnimationStep*						m_curr_step;		// current step in current animation
			SpriteAnimationSequence*					m_curr_animation;	// current animation playing
			float										m_step_in_anim;		// current step in current animation playing
			String										m_default_anim;		// default animation to return to when completing cycle
			bool										m_loop;				// should we loop on the current animation
			float										m_time_to_wait;		// time to wait on current step

		public:
			// target - target sprite to animate
			// animations - all the character animations
			NESSENGINE_API AnimatorSpriteCharacter(const SpritePtr& target, const SharedPtr<AnimatorSpritesheetAnimations>& animations) :
			TargetAnimatorAPI(target), m_animations(animations), m_speed(1.0f), m_curr_step(nullptr), m_curr_animation(nullptr), m_step_in_anim(0.0f), m_loop(false), m_time_to_wait(0.0f)
			{
			}

			// set/get animator speed
			NESSENGINE_API inline void set_speed(float speed) {m_speed = speed;}
			NESSENGINE_API inline float get_speed() {return m_speed;}

			// set/get default animation
			NESSENGINE_API inline void set_default_animation(const String& animName) {m_default_anim = animName;}
			NESSENGINE_API inline const String& get_default_animation() {return m_default_anim;}

			// start playing an animation
			// animationName is the name of the animation to play
			// if loop is true it will continue to play this animation in an endless loop until other animation is set
			NESSENGINE_API void play_animation(const String& animationName, bool loop)
			{
				// set loop
				m_loop = loop;

				// set current animation
				m_curr_animation = &((*m_animations)[animationName]);

				// reset the new animation
				reset_current_animation();
			}

			// reset the currently playing animation back to the starting point
			NESSENGINE_API void reset_current_animation()
			{
				m_step_in_anim = 0.0f;
				m_time_to_wait = 0.0f;
				m_curr_step = nullptr;
			}

			// end the currently playing animation.
			// if animation set to loop it will be equevilent to reset_current_animation().
			// if not in loop then if there is default animation it will switch to default, else it will
			// simple get "stuck" on the last step of the animation.
			NESSENGINE_API void end_current_animation()
			{
				// if animation is in loop, reset the current animation
				if (m_loop)
				{
					reset_current_animation();
				}
				// if animation is not in loop, either return to default or set to last step
				else
				{
					if (m_default_anim.length() > 0)
					{
						play_animation(m_default_anim, true);
					}
					else
					{
						m_step_in_anim = (float)(m_curr_animation->steps.size() - 1);
					}
				}
			}

			// do the animation
			NESSENGINE_API virtual void do_animation(Renderer* renderer)
			{
				// if no animation is playing nothing to do
				if (m_curr_animation == nullptr)
					return;

				// get current step index
				unsigned int curr_step_int = (unsigned int)floorf(m_step_in_anim);

				// check if animation ended
				if (curr_step_int >= m_curr_animation->steps.size())
				{
					end_current_animation();
				}
				else
				{
					// increase current animation step
					if (m_time_to_wait > 0.0f)
					{
						m_time_to_wait -= renderer->time_factor() * m_speed;
					}
					else
					{
						m_step_in_anim += renderer->time_factor() * m_speed;
					}
				}

				// get current animation step data
				SpriteAnimationStep* newStep = &m_curr_animation->steps[curr_step_int];

				// check if we changed the current animation step
				if (newStep != m_curr_step)
				{
					m_curr_step = newStep;
					m_time_to_wait = m_curr_step->duration;
					(ness_ptr_cast<Sprite>(m_target))->set_source_from_sprite_sheet(m_curr_step->position, m_curr_animation->texture_steps_count);
				}
			}
		};

		// define the animator pointer
		NESSENGINE_API typedef SharedPtr<AnimatorSpriteCharacter> AnimatorSpriteCharacterPtr;

	};
};