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
* A sprite that is also an animator and you can attach animators to.
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "sprite.h"
#include "../../animators/animator_api.h"
#include "../../animators/animators_queue.h"

namespace Ness
{

	/*
	* this sprite is a mash-up between a sprite and an animator.
	* the result is a sprite you can register as an active animator and attach animators on it.
	* this is useful for creating sprites with lots of animators on them and use them as a single unit.
	*
	* example of usage:
	*
	* AnimatedSpritePtr sprite = node->create_animated_sprite("bla.jpg");
	* node->renderer()->register_animator(sprite);
	* Ness::Animators::AnimatorSpritePtr animator = ness_make_ptr<Ness::Animators::AnimatorSprite>(sprite, ...);
    * sprite->register_animator(animator);
	* 
	*/
	class AnimatedSprite : public Sprite, public Animators::AnimatorAPI, public Animators::AnimatorsQueue
	{
	protected:
		
	public:

		// create the animated sprite with or without texture
		// animatorsQueuePtr is a pointer to animators queue to append this animated sprite to. if nullptr, will not register to any animators queue and
		// you will need to register manually.
		NESSENGINE_API AnimatedSprite(Renderer* renderer, ManagedResources::ManagedTexturePtr texture, Animators::AnimatorsQueue* animatorsQueuePtr);
		NESSENGINE_API AnimatedSprite(Renderer* renderer, const String& TextureFile, Animators::AnimatorsQueue* animatorsQueuePtr);
		NESSENGINE_API AnimatedSprite(Renderer* renderer, Animators::AnimatorsQueue* animatorsQueuePtr);

		NESSENGINE_API ~AnimatedSprite();

		// activate all the animators on this sprite
		NESSENGINE_API virtual void do_animation(Renderer* renderer);
	};

	// sprite pointer type
	NESSENGINE_API typedef SharedPtr<AnimatedSprite> AnimatedSpritePtr;
};