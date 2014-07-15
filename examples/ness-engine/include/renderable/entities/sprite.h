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
		bool	is_static;
		Point	anchor;
		bool	alpha_channels;

		SSpriteDefaults()
		{
			is_static = false;
			anchor = Point::ZERO;
			alpha_channels = false;
		}
	};

	// the renderable sprite class
	class Sprite : public Entity
	{
	protected:
		ManagedResources::ManagedTexturePtr		m_texture;
		Rectangle								m_source_rect;
		
	public:

		// defaults
		NESSENGINE_API static SSpriteDefaults Defaults;

		// create the sprite with texture
		NESSENGINE_API Sprite(Renderer* renderer, NodeAPI* parent, ManagedResources::ManagedTexturePtr texture);
		NESSENGINE_API Sprite(Renderer* renderer, NodeAPI* parent, const std::string& TextureFile);
		NESSENGINE_API Sprite(Renderer* renderer, NodeAPI* parent);

		// set defaults (based on Sprite::Defaults)
		NESSENGINE_API virtual void set_defaults();

		// change texture
		NESSENGINE_API inline void change_texture(ManagedResources::ManagedTexturePtr NewTexture) {m_texture = NewTexture;}

		// set source as rect
		NESSENGINE_API void set_source_rect(const Rectangle& srcRect);

		// return the texture this sprite uses
		NESSENGINE_API inline const ManagedResources::ManagedTexturePtr& get_texture() const {return m_texture;}
		NESSENGINE_API inline ManagedResources::ManagedTexturePtr get_texture() {return m_texture;}

		// set source rect from sprite sheet
		// for example, if your sprite sheet contains 4x8 animation steps, and you want to set sprite to step (3,2), use it like so:
		//		set_source_from_sprite_sheet(Pointi(3,2), Sizei(4,8));
		// step: the index of the animation step inside the sprite sheet (x, y)
		// stepsCount: how many total animation steps the sheet has (x, y)
		NESSENGINE_API void set_source_from_sprite_sheet(const Pointi& step, const Sizei stepsCount);

		// reset source rect to be full image size
		NESSENGINE_API void reset_source_rect();

	protected:

		// init the sprite from a given texture
		void init_from_texture(ManagedResources::ManagedTexturePtr texture);

		// the actual rendering function
		NESSENGINE_API virtual void do_render(const Rectangle& target, const SRenderTransformations& transformations);
	};

	// sprite pointer type
	NESSENGINE_API typedef NessSharedPtr<Sprite> SpritePtr;
};