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

namespace Ness
{
	namespace Animators
	{

		// represent a single step inside an animation sequence
		struct SpriteAnimationStep
		{
			Pointi position;		// the index of the animation step inside the spritesheet
			float duration;			// for how long (in seconds) to wait on this step before moving to the next step
		};

		// represent an entire animation sequence, composed of steps and optional texture
		struct SpriteAnimationSequence
		{
			String change_texture;								// optional, if not empty string will change to new texture
			Pointi texture_steps_count;							// optional, if not Pointi::ZERO will reset the number of steps in the texture spritesheet (use if you change texture)
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
			SpriteAnimationSequence*					m_curr_animation;	// current animation playing
			float										m_step_in_anim;		// current step in current animation playing

		public:
			// target - target sprite to animate
			// animations - all the character animations
			NESSENGINE_API AnimatorSpriteCharacter(const SpritePtr& target, const SharedPtr<AnimatorSpritesheetAnimations>& animations) :
			TargetAnimatorAPI(target), m_animations(animations), m_speed(1.0f)
			{
			}

			// set/get animator speed
			NESSENGINE_API inline void set_speed(float speed) {m_speed = speed;}
			NESSENGINE_API inline float get_speed() {return m_speed;}


			NESSENGINE_API virtual void do_animation(Renderer* renderer)
			{
				
			}
		};

		// define the animator pointer
		NESSENGINE_API typedef SharedPtr<AnimatorSpriteCharacter> AnimatorSpriteCharacterPtr;

	};
};