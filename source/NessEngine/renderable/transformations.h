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
* Define all the possible transformations that may be applied on an entity or a node object
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <SDL.h>
#include "../exports.h"
#include "../basic_types/all_basic_types.h"
namespace Ness
{
	// possible blend modes
	enum EBlendModes
	{
		BLEND_MODE_NONE = SDL_BLENDMODE_NONE,
		BLEND_MODE_BLEND = SDL_BLENDMODE_BLEND,
		BLEND_MODE_ADD = SDL_BLENDMODE_ADD,
		BLEND_MODE_MOD = SDL_BLENDMODE_MOD
	};

	// rendering transformation (rendering settings you can apply on a renderable object
	struct SRenderTransformations
	{
		Point				position;
		Size				scale;
		float				rotation;
		EBlendModes			blend;
		Color				color;
		float				zorder;

		NESSENGINE_API SRenderTransformations(Point Position = Size(0, 0), Size Scale = Size(1, 1), float Rotation = 0.0f,  
								EBlendModes Blend = BLEND_MODE_NONE, Color RenderColor = Color::WHITE, float Zorder = 0.0f) :
				position(Position), scale(Scale), rotation(Rotation), blend(Blend), color(RenderColor), zorder(Zorder)
		{
		}

		// combine other transformations into this transformations
		NESSENGINE_API void add_transformations(const SRenderTransformations& other)
		{
			position += other.position;
			scale *= other.scale;
			rotation += other.rotation;
			if ((int)other.blend > (int)blend) blend = other.blend;
			color *= other.color;
			zorder += other.zorder;
		}
	};
};