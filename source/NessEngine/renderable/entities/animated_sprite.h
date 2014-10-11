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

namespace Ness
{

	/*
	* this sprite is a mash-up between a sprite and an animator.
	* the result is a sprite you can register as an active animator and attach animators on it.
	* this is useful for creating sprites with lots of animators on them and use them as a single unit.
	*/
	class AnimatedSprite : public Sprite, public Animators::AnimatorAPI
	{
	protected:
		
	public:

		// create the animated sprite with or without texture
		NESSENGINE_API AnimatedSprite(Renderer* renderer, ManagedResources::ManagedTexturePtr texture) : Sprite(renderer, texture) {}
		NESSENGINE_API AnimatedSprite(Renderer* renderer, const String& TextureFile) : Sprite(renderer, TextureFile) {}
		NESSENGINE_API AnimatedSprite(Renderer* renderer) : Sprite(renderer) {}

	};

	// sprite pointer type
	NESSENGINE_API typedef SharedPtr<AnimatedSprite> AnimatedSpritePtr;
};