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

#include "animated_sprite.h"
#include "../../renderer/renderer.h"
#include "../../animators/animators_queue.h"

namespace Ness
{
	void AnimatedSprite::do_animation(Renderer* renderer)
	{
		do_animations();
	}

	// create the animated sprite with or without texture
	AnimatedSprite::AnimatedSprite(Renderer* renderer, ManagedResources::ManagedTexturePtr texture, Animators::AnimatorsQueue* animatorsQueuePtr)
		: Animators::AnimatorsQueue(renderer), Sprite(renderer, texture) 
	{
		if (animatorsQueuePtr)
		{
			animatorsQueuePtr->__register_animator_unsafe(this);
		}
	}

	AnimatedSprite::AnimatedSprite(Renderer* renderer, const String& TextureFile, Animators::AnimatorsQueue* animatorsQueuePtr)
		: Animators::AnimatorsQueue(renderer), Sprite(renderer, TextureFile) 
	{
		if (animatorsQueuePtr)
		{
			animatorsQueuePtr->__register_animator_unsafe(this);
		}
	}

	AnimatedSprite::AnimatedSprite(Renderer* renderer, Animators::AnimatorsQueue* animatorsQueuePtr)
		: Animators::AnimatorsQueue(renderer), Sprite(renderer) 
	{
		if (animatorsQueuePtr)
		{
			animatorsQueuePtr->__register_animator_unsafe(this);
		}
	}

	AnimatedSprite::~AnimatedSprite()
	{
		if (m_animator_queue_parent)
		{
			m_animator_queue_parent->__remove_animator_unsafe(this);
		}
	}
};