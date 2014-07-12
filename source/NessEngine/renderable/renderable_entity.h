/**
* A renderable object with basic optimizations, size and anchor point. still an abstract class.
* This class is used for renderables that are actually objects, rather than nodes, tilemaps, or other composite renderables.
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "renderable.h"
#include "../primitives/rectangle.h"

namespace Ness
{

	// the renderable sprite class
	class RenderableEntity : public Renderable
	{
	protected:
		Size									m_size;
		Point									m_anchor;
		bool									m_static;
		Rectangle								m_target_rect;
		bool									m_need_update;
		SRenderTransformations					m_absolute_transformations;

	public:

		// create the sprite with texture
		NESSENGINE_API RenderableEntity(Renderer* renderer, RenderableParent* parent);

		// called whenever transformations are updated
		NESSENGINE_API virtual void transformations_update();

		// set source as rect
		NESSENGINE_API void set_source_rect(const Rectangle& srcRect);

		// set entity base size
		// note: this is the basic size that is multiplied by scale. size should be the basic sprite size and remain pretty much const,
		// scale is something you want to play with more often to enlare or shrink the sprite
		NESSENGINE_API void set_size(const Size& NewSize) {m_size = NewSize;}
		NESSENGINE_API inline const Size& get_size() const {return m_size;}

		// check if this sprite really is visible: if it's currently visible, opacity > 0, and inside screen bounderies
		NESSENGINE_API virtual bool is_really_visible(const CameraPtr& camera = NullCamera);

		// set/get if this sprite is static (static sprites are more efficient but don't take parent transformation or camera into consideration.
		NESSENGINE_API inline void set_static(bool IsStatic) {m_static = IsStatic;}
		NESSENGINE_API inline bool is_static() const {return m_static;}

		// set the anchor point
		// anchor is a point with values from 0.0 to 1.0, that represent the rotation center of this sprite (multiplied by his size)
		// for example, anchor of (0.5, 1), will rotate the sprite from his center bottom
		NESSENGINE_API inline void set_anchor(const Point& NewAnchor) {m_anchor = NewAnchor;}

		// return the target rectangle this sprite will be rendered on
		NESSENGINE_API const Rectangle& get_target_rect() const {return m_target_rect;}

		// get absolute transformations for this renderable sprite
		NESSENGINE_API virtual const SRenderTransformations& get_absolute_transformations();

		// reset source rect to be full image size
		NESSENGINE_API void reset_source_rect();

		// render this sprite
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera);

	protected:

		// the actual rendering function to override
		// target: target rectangle to render to (final, with camera and everything calculated)
		// transformations: absolute final transformations to render with (with parents included)
		NESSENGINE_API virtual void do_render(const Rectangle& target, const SRenderTransformations& transformations) = 0;

		// calculate target rect
		virtual void calc_target_rect();
	};
};