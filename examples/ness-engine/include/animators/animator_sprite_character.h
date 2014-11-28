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
* NOTE THIS CLASS IS NOT TESTED YET AND MAY NOT WORK!!!! TBD!!!
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
		// read about SpriteAnimationSequence to understand what an animation sequence is.
		struct SpriteAnimationStep
		{
			Pointi position;		// the index of the animation step inside the spritesheet
			float duration;			// for how long to wait (in seconds) on this step before moving to the next step

			// ctor that get position in texture file and duration for this step
			SpriteAnimationStep(const Pointi& Position, float Duration = 0.15f) : position(Position), duration(Duration) {}
		};

		// represent an entire animation sequence, composed of steps and optional texture
		// for example, the entire animation loop of a character walking right would be represented by this struct.
		struct SpriteAnimationSequence
		{
			Pointi texture_steps_count;							// how many steps there are in total in the texture spritesheet (set this even if not changing texture)
			Containers::Vector<SpriteAnimationStep> steps;		// an array of steps in this animation (every step is index in the texture file and how long to wait on it)
			String change_texture;								// optional, if not empty string will switch to the given texture once this animation starts

			SpriteAnimationSequence() {}
			SpriteAnimationSequence(const Pointi& steps_count, const String& texture = "") : texture_steps_count(steps_count), change_texture(texture) {}
		};

		// a container that holds a set of animation sequences with names.
		// for example, you can assign the unique name "walk_right" to a SpriteAnimationSequence instance 
		// that will define all the necassery steps to play the walking-right animation cycle.
		class CharacterAnimations
		{
		private:
			Containers::UnorderedMap<String, SpriteAnimationSequence> m_animations;

		public:
			// add new animation to this character animations
			// name is the name of the animation (unique identifier)
			// animationSequence is instance of SpriteAnimationSequence with all the animation data
			NESSENGINE_API inline void add_animation(const String& name, const SpriteAnimationSequence& animationSequence) 
				{m_animations[name] = animationSequence;}

			// get animation sequence by name
			NESSENGINE_API inline const SpriteAnimationSequence& get_animation(const String& name) {return m_animations[name];}

		};
		typedef SharedPtr<CharacterAnimations> CharacterAnimationsPtr;

		// sprite from spritesheet animation
		// NOTE THIS CLASS IS NOT TESTED YET AND MAY NOT WORK!!!! TBD!!!
		class AnimatorSpriteCharacter : public TargetAnimatorAPI
		{
		private:
			CharacterAnimationsPtr						m_animations;		// all the animation sequences (must be defined outside)
			float										m_speed;			// animations speed factor
			const SpriteAnimationStep*					m_curr_step;		// current step in current animation
			const SpriteAnimationSequence*				m_curr_animation;	// current animation playing
			unsigned int								m_step_in_anim;		// current step in current animation playing
			String										m_default_anim;		// default animation to return to when completing cycle
			bool										m_loop;				// should we loop on the current animation
			float										m_time_to_wait;		// time to wait on current step
			Ness::Pointi								m_override_index;	// if set to different then (-1, -1), will override the animation index

		public:
			NESSENGINE_API static CharacterAnimationsPtr create_empty_animations() {return ness_make_ptr<CharacterAnimations>();}

		public:
			// target - target sprite to animate
			// animations - all the character animations. if not provided, will create an empty animations set you can later access via get_animations()
			NESSENGINE_API AnimatorSpriteCharacter(const SpritePtr& target, const CharacterAnimationsPtr& animations = CharacterAnimationsPtr()) :
			TargetAnimatorAPI(target), m_animations(animations), m_speed(1.0f), m_curr_step(nullptr), m_curr_animation(nullptr), m_step_in_anim(0), 
				m_loop(false), m_time_to_wait(0.0f), m_override_index(-1, -1)
			{
				if (m_animations.get() == nullptr)
				{
					m_animations = ness_make_ptr<CharacterAnimations>();
				}
			}

			// get the animations set
			NESSENGINE_API inline const CharacterAnimationsPtr& get_animations() const {return m_animations;}
			NESSENGINE_API inline CharacterAnimationsPtr get_animations() {return m_animations;}

			// set/get animator speed
			NESSENGINE_API inline void set_speed(float speed) {m_speed = speed;}
			NESSENGINE_API inline float get_speed() {return m_speed;}

			// set value for x or y (or both) that will override the position in texture file provided by the current animation step.
			// what this is good for: imagine you have an old-school rpg game, and you have a spritesheet for character walking animation. it
			// makes perfect sense that in your spritesheet the first row will be used for up direction, second row for down, third for left and 
			// forth for right. since all the walking animations are built the same but have different Y position in the texture file, you can use
			// this feature to define only 1 animation sequence, "walking", and play with the override y position to change character direction.
			NESSENGINE_API inline void set_override_position_x(int alt_index) {m_override_index.x = alt_index;}
			NESSENGINE_API inline void set_override_position_y(int alt_index) {m_override_index.y = alt_index;}
			NESSENGINE_API inline void set_override_position(const Ness::Pointi& alt_index) {m_override_index = alt_index;}
			NESSENGINE_API inline void clear_override_position() {m_override_index = Ness::Pointi(-1, -1);}

			// set/get default animation
			NESSENGINE_API inline void set_default_animation(const String& animName) {m_default_anim = animName;}
			NESSENGINE_API inline const String& get_default_animation() {return m_default_anim;}

			// play the default animation in an endless loop (until different animation is set)
			NESSENGINE_API inline void play_default_animation() {play_animation(m_default_anim, true);}

			// start playing an animation
			// animationName is the name of the animation to play
			// if loop is true it will continue to play this animation in an endless loop until other animation is set
			// if dontResetIfSame is true, it will not reset the animation if this is the animation currently playing
			NESSENGINE_API void play_animation(const String& animationName, bool loop, bool dontResetIfSame = true)
			{

				// set loop
				m_loop = loop;

				// get new animation to play
				const SpriteAnimationSequence* newAnimation = &((*m_animations).get_animation(animationName));

				// same and shouldn't reset? if so return
				if (m_curr_animation == newAnimation && dontResetIfSame)
				{
					return;
				}

				// set current animation
				m_curr_animation = newAnimation;

				// reset the new animation
				reset_current_animation();
			}

			// reset the currently playing animation back to the starting point
			NESSENGINE_API void reset_current_animation()
			{
				m_step_in_anim = 0;
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
						play_default_animation();
					}
					else
					{
						m_step_in_anim = (m_curr_animation->steps.size() - 1);
					}
				}
			}

			// do the animation
			NESSENGINE_API virtual void do_animation(Renderer* renderer)
			{
				// if no animation is playing nothing to do
				if (m_curr_animation == nullptr)
				{
					if (m_default_anim.length() > 0)
					{
						play_default_animation();
					}
					return;
				}
				
				// increase current animation step
				m_time_to_wait -= renderer->time_factor() * m_speed;
				
				// if end of time for this step advance to next step (or steps, it might skip some steps)
				while (m_time_to_wait <= 0)
				{
					// advance to next step
					m_step_in_anim++;

					// check if animation ended
					if (m_step_in_anim >= m_curr_animation->steps.size())
					{
						m_step_in_anim = 0;
					}

					// get new step data
					m_curr_step = &m_curr_animation->steps[m_step_in_anim];
					m_time_to_wait += m_curr_step->duration;
					if (m_time_to_wait > 0.0f)
					{
						Ness::Pointi pos = m_curr_step->position;
						if (m_override_index.x != -1) pos.x = m_override_index.x;
						if (m_override_index.y != -1) pos.y = m_override_index.y;
						(ness_ptr_cast<Sprite>(m_target))->set_source_from_sprite_sheet(pos, m_curr_animation->texture_steps_count);
					}
				}

				// get current animation step data
				const SpriteAnimationStep* newStep = &m_curr_animation->steps[m_step_in_anim];
			}
		};

		// define the animator pointer
		NESSENGINE_API typedef SharedPtr<AnimatorSpriteCharacter> AnimatorSpriteCharacterPtr;

	};
};