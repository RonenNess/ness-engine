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