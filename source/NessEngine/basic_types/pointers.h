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
* Alias to the Ness-Engine pointer type, which is shared_ptr by default.
*
* Author: Ronen Ness
* Since:  26/03/14
*/

#pragma once
#include "../exports.h"
#include <memory>

namespace Ness
{
	// ness-engine shared ptr
	#define SharedPtr std::shared_ptr

	// create a shared ptr.
	// usage: SpritePtr NewSprite = ness_make_ptr<Sprite>(renderer, "sometexture.png");
	#define ness_make_ptr std::make_shared

	// cast a ness-pointer.
	// usage: Ness::SpritePtr sprite = ness_ptr_cast<Ness::Sprite>( SomeRenderablePtr );
	// will return empty ptr if type cannot be cast to sprite.
	#define ness_ptr_cast std::dynamic_pointer_cast
};