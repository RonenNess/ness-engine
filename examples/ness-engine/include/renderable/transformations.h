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