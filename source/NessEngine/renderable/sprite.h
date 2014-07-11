/**
* A renderable sprite
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "renderable.h"
#include "../resources/managed_texture.h"
#include "../primitives/rectangle.h"

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
			anchor = Point::HALF;
			alpha_channels = false;
		}
	};

	// the renderable sprite class
	class Sprite : public Renderable
	{
	protected:
		ManagedResources::ManagedTexturePtr		m_texture;
		Rectangle								m_source_rect;
		Size									m_size;
		Point									m_anchor;
		bool									m_static;
		Rectangle								m_target_rect;
		bool									m_need_update;
		SRenderTransformations					m_absolute_transformations;

	public:

		// defaults
		static SSpriteDefaults Defaults;

		// create the sprite with texture
		Sprite(Renderer* renderer, RenderableParent* parent, ManagedResources::ManagedTexturePtr texture);
		Sprite(Renderer* renderer, RenderableParent* parent, const std::string& TextureFile);
		Sprite(Renderer* renderer, RenderableParent* parent);

		// set defaults (based on Sprite::Defaults)
		void set_defaults();

		// change texture
		inline void change_texture(ManagedResources::ManagedTexturePtr NewTexture) {m_texture = NewTexture;}

		// called whenever transformations are updated
		virtual void transformations_update();

		// set source as rect
		void set_source_rect(const Rectangle& srcRect);

		// return the texture this sprite uses
		inline const ManagedResources::ManagedTexturePtr& get_texture() const {return m_texture;}
		inline ManagedResources::ManagedTexturePtr get_texture() {return m_texture;}

		// set sprite size
		// note: this is the basic size that is multiplied by scale. size should be the basic sprite size and remain pretty much const,
		// scale is something you want to play with more often to enlare or shrink the sprite
		void set_size(const Size& NewSize) {m_size = NewSize;}

		// check if this sprite really is visible: if it's currently visible, opacity > 0, and inside screen bounderies
		virtual bool is_really_visible(const CameraPtr& camera = NullCamera);

		// set/get if this sprite is static (static sprites are more efficient but don't take parent transformation or camera into consideration.
		inline void set_static(bool IsStatic) {m_static = IsStatic;}
		inline bool is_static() const {return m_static;}

		// set the anchor point
		// anchor is a point with values from 0.0 to 1.0, that represent the rotation center of this sprite (multiplied by his size)
		// for example, anchor of (0.5, 1), will rotate the sprite from his center bottom
		inline void set_anchor(const Point& NewAnchor) {m_anchor = NewAnchor;}

		// set source rect from sprite sheet
		// for example, if your sprite sheet contains 4x8 animation steps, and you want to set sprite to step (3,2), use it like so:
		//		set_source_from_sprite_sheet(Pointi(3,2), Sizei(4,8));
		// step: the index of the animation step inside the sprite sheet (x, y)
		// stepsCount: how many total animation steps the sheet has (x, y)
		void set_source_from_sprite_sheet(const Pointi& step, const Sizei stepsCount);

		// return the target rectangle this sprite will be rendered on
		const Rectangle& get_target_rect() const {return m_target_rect;}

		// get absolute transformations for this renderable sprite
		virtual const SRenderTransformations& get_absolute_transformations();

		// reset source rect to be full image size
		void reset_source_rect();

		// render this sprite
		virtual void render(const CameraPtr& camera = NullCamera);

	protected:

		// init the sprite from a given texture
		void init_from_texture(ManagedResources::ManagedTexturePtr texture);

		// calculate target rect
		void calc_target_rect();
	};

	// sprite pointer type
	typedef std::shared_ptr<Sprite> SpritePtr;
};