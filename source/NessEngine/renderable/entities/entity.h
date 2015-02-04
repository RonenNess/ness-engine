/**
* Base class for every scene entity with cached transformations
* Author: Ronen Ness
* Since: 07/1014
*/

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

#pragma once
#include "../entity_api.h"
#include "../transformable_api.h"
#include "../../basic_types/rectangle.h"

namespace Ness
{

	// base class for every renderable entity.
	// manage caching of transformations and inherit transformations from parents, calculating target rectangle, static/non-static and anchor logic.
	// if you want to create a renderable entity in ness-engine that will behave like the other engine entities, it's recommended to inherit from this class!
	class Entity : public EntityAPI
	{
	protected:
		Size									m_size;								// size of this entity
		Point									m_anchor;							// anchor point relative to entity size
		Rectangle								m_target_rect;						// target rectagnle we render this entity on screen
		bool									m_need_transformations_update;		// do we need to update the transforlmations cache?
		SRenderTransformations					m_absolute_transformations;			// transformations cache, calculated with parents
		unsigned int							m_last_render_frame_id;				// return the frame id of the last time this entity was really rendered
		unsigned int							m_last_update_frame_id;				// return the frame id of the last time this entity was updated
		unsigned char							m_highlight;						// how many highlight passes to do on this object

	public:

		// create the sprite with texture
		NESSENGINE_API Entity(Renderer* renderer);

		// set the flag of this entity that it needs transformations update. don't call this yourself, ness-engine should call it automatically.
		NESSENGINE_API virtual void transformations_update();

		// return if need transformations udpate
		NESSENGINE_API virtual bool need_transformations_update() {return m_need_transformations_update;}

		// return the last frame this entity was really rendered
		NESSENGINE_API virtual unsigned int get_last_rendered_frame_id() const { return m_last_render_frame_id; }
		NESSENGINE_API virtual bool was_rendered_this_frame() const;

		// get the last frame in which this entity was updated
		NESSENGINE_API virtual inline unsigned int get_last_update_frame_id() const { return m_last_update_frame_id; };
		NESSENGINE_API virtual bool was_updated_this_frame() const;

		// enable/disable highlight
		NESSENGINE_API inline void set_highlight(unsigned char number_of_passes) { m_highlight = number_of_passes; }

		// set entity base size
		// note: this is the basic size that is multiplied by scale. size should be the basic sprite size and remain pretty much const,
		// scale is something you want to play with more often to enlare or shrink the sprite
		NESSENGINE_API inline void set_size(const Size& NewSize) { m_size = NewSize; transformations_update(); }
		NESSENGINE_API inline void set_size(float size) { set_size(Point::ONE * size); }
		NESSENGINE_API inline const Size& get_size() const {return m_size;}

		// return absolute size
		NESSENGINE_API inline Size get_absolute_size() {return get_absolute_transformations().scale * m_size;}
		NESSENGINE_API inline Size get_absolute_size_const() const {return get_absolute_transformations_const().scale * m_size;}

		// return absolute position
		NESSENGINE_API inline const Point& get_absolute_position() {return get_absolute_transformations().position;}
		NESSENGINE_API inline const Point& get_absolute_position_const() const {return get_absolute_transformations_const().position;}

		// return absolute rotation
		NESSENGINE_API inline float get_absolute_rotation() {return get_absolute_transformations().rotation;}
		NESSENGINE_API inline float get_absolute_rotation_const() const {return get_absolute_transformations_const().rotation;}

		// return absolute direction vector
		NESSENGINE_API inline Point get_absolute_direction_vector() {return Point::from_angle(get_absolute_transformations().rotation);}
		NESSENGINE_API inline Point get_absolute_direction_vector_const() const {return Point::from_angle(get_absolute_transformations_const().rotation);}

		// get absolute zindex
		NESSENGINE_API inline float get_absolute_zindex() {return get_absolute_transformations().zorder;}
		NESSENGINE_API inline float get_absolute_zindex_const() const {return get_absolute_transformations_const().zorder;}

		// check if this sprite really is visible: if it's currently visible, opacity > 0, and inside screen bounderies
		NESSENGINE_API virtual bool is_really_visible(const CameraApiPtr& camera);

		// check if this sprite really is visible, but without changing its internal state
		NESSENGINE_API virtual bool is_really_visible_const(const CameraApiPtr& camera) const;

		// set the anchor point
		// anchor is a point with values from 0.0 to 1.0, that represent the rotation center of this sprite (multiplied by his size)
		// for example, anchor of (0.5, 1), will rotate the sprite from his center bottom
		NESSENGINE_API inline void set_anchor(const Point& NewAnchor) {m_anchor = NewAnchor;}
		NESSENGINE_API inline const Point& get_anchor() const {return m_anchor;}

		// return the target rectangle this sprite will be rendered on, not including camera position
		NESSENGINE_API virtual const Rectangle& get_last_target_rect() const {return m_target_rect;}

		// get absolute transformations for this renderable sprite
		NESSENGINE_API virtual const SRenderTransformations& get_absolute_transformations();

		// a const version of get_absolute_transformations, which does not affect the inner state of the entity (but might return information not fully up-to-date)
		NESSENGINE_API virtual const SRenderTransformations& get_absolute_transformations_const() const;

		// render this entity
		NESSENGINE_API virtual void render(const CameraApiPtr& camera);

		// calculate the target rect, which is the position and size of this entity when rendered on the screen
		NESSENGINE_API virtual void calc_target_rect();

	protected:

		// the actual rendering function to override
		// target: target rectangle to render to (final, with camera and everything calculated)
		// transformations: absolute final transformations to render with (with parents included)
		NESSENGINE_API virtual void do_render(const Rectangle& target, const SRenderTransformations& transformations) = 0;
	};
};