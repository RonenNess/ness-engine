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
* A renderable sprite
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "entity.h"
#include "../../managed_resources/managed_texture.h"
#include "../../basic_types/rectangle.h"

namespace Ness
{
	// default properties you can set to all future sprites
	struct SSpriteDefaults
	{
		bool	is_static;				// if true, will make all sprites you create static by default
		Point	anchor;					// set default anchor point for newly created sprites
		bool	alpha_channels;			// if true, will make all new sprites blending mode set to BLEND by default

		SSpriteDefaults()
		{
			is_static = false;
			anchor = Point::ZERO;
			alpha_channels = false;
		}
	};

	// the renderable sprite class - render textures on screen
	class Sprite : public Entity
	{
	protected:
		ManagedResources::ManagedTexturePtr		m_texture;
		Rectangle								m_source_rect;
		
	public:

		// defaults
		NESSENGINE_API static SSpriteDefaults Defaults;

		// create the sprite with texture
		NESSENGINE_API Sprite(Renderer* renderer, ManagedResources::ManagedTexturePtr texture);
		NESSENGINE_API Sprite(Renderer* renderer, const String& TextureFile);
		NESSENGINE_API Sprite(Renderer* renderer);

		// set defaults (based on Sprite::Defaults)
		NESSENGINE_API virtual void set_defaults();

		// change texture
		// if resetSizeAndSource == true, it will also set the size of the sprite to the whole size of the texture and the source
		// rect to be the entire texture size.
		NESSENGINE_API void change_texture(ManagedResources::ManagedTexturePtr NewTexture, bool resetSizeAndSource = true);
		NESSENGINE_API void change_texture(const String& NewTextureFile, bool resetSizeAndSource = true);

		// set texture source rect (to render entire texture give (0, 0, textureSize.x, textureSize.y)
		NESSENGINE_API void set_source_rect(const Rectangle& srcRect);
		NESSENGINE_API inline const Rectangle& get_source_rect() const {return m_source_rect;}

		// return the texture this sprite uses
		NESSENGINE_API inline const ManagedResources::ManagedTexturePtr& get_texture() const {return m_texture;}
		NESSENGINE_API inline ManagedResources::ManagedTexturePtr get_texture() {return m_texture;}

		// set source rect from sprite sheet
		// for example, if your sprite sheet contains 4x8 animation steps, and you want to set sprite to step (3,2), use it like so:
		//		set_source_from_sprite_sheet(Pointi(3,2), Sizei(4,8));
		// step: the index of the animation step inside the sprite sheet (x, y)
		// stepsCount: how many total animation steps the sheet has (x, y)
		// if setSize == true, it will also set the size of the sprite to fit a single step of the sprite sheet
		NESSENGINE_API void set_source_from_sprite_sheet(const Pointi& step, const Sizei stepsCount, bool setSize = false);

		// reset source rect to be full image size
		NESSENGINE_API void reset_source_rect();

	protected:

		// the actual rendering function
		NESSENGINE_API virtual void do_render(const Rectangle& target, const SRenderTransformations& transformations);
	};

	// sprite pointer type
	NESSENGINE_API typedef SharedPtr<Sprite> SpritePtr;
};