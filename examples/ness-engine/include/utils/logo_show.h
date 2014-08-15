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
* Implement function to show logo screen on a given scene.
* Logo screen is basically a node that covers the entire screen with a given color and puts a texture (logo in the middle), then fades out.
* this function only wraps the creation of such object for you
* Author: Ronen Ness
* Since: 08/1014
*/

#pragma once
#include "../renderable/nodes/node.h"
#include "../exports.h"

namespace Ness
{
	namespace Utils
	{
		NESSENGINE_API void make_logo_screen(const Ness::NodePtr& parentNode, 
			const String& textureName = "ness-engine/resources/gfx/logo.png", 
			const Ness::Color& backColor = Ness::Color::BLACK, 
			float timeUntilFadeout = 1.5f, 
			float fadeOutSpeed = 0.75f);
	}
}