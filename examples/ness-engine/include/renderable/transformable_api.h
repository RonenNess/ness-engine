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
* This entity is a middle layer - gives the logic of transformations (color, scale, blending, etc..)
* every object that inherit from this will support transformations, but will also require to implement some functions
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "transformations.h"

namespace Ness
{

	// a renderable object/node
	class Transformable
	{
	protected:
		SRenderTransformations	m_transformations;				// render transformations (opacity, size, position, ...)

	public:

		// setters/getters
		NESSENGINE_API virtual void set_transformations(const SRenderTransformations& newTrans) {m_transformations = newTrans; transformations_update();}
		NESSENGINE_API virtual const SRenderTransformations& get_transformation() const {return m_transformations;}
		NESSENGINE_API virtual void set_position(const Point& new_pos) {if (new_pos == m_transformations.position) return; m_transformations.position = new_pos; transformations_update();}
		NESSENGINE_API virtual void add_position(const Point& new_pos) {m_transformations.position += new_pos; transformations_update();}
		NESSENGINE_API virtual void multiply_position(const Point& new_pos) {m_transformations.position *= new_pos; transformations_update();}
		NESSENGINE_API virtual void set_scale(const Size& new_scale) {if (new_scale == m_transformations.scale) return; m_transformations.scale = new_scale; transformations_update();}
		NESSENGINE_API virtual void add_scale(const Size& new_scale) {m_transformations.scale += new_scale; transformations_update();}
		NESSENGINE_API virtual void multiply_scale(const Size& new_scale) {m_transformations.scale *= new_scale; transformations_update();}
		NESSENGINE_API virtual void set_scale(float new_scale) {set_scale(Point(new_scale, new_scale));}
		NESSENGINE_API virtual void add_scale(float new_scale) {m_transformations.scale += new_scale; transformations_update();}
		NESSENGINE_API virtual void multiply_scale(float new_scale) {m_transformations.scale *= new_scale; transformations_update();}
		NESSENGINE_API virtual void set_rotation(float new_rotation) {if (new_rotation == m_transformations.rotation) return; m_transformations.rotation = new_rotation; transformations_update();}
		NESSENGINE_API virtual void add_rotation(float new_rotation) {m_transformations.rotation += new_rotation; transformations_update();}
		NESSENGINE_API virtual void multiply_rotation(float new_rotation) {m_transformations.rotation *= new_rotation; transformations_update();}
		NESSENGINE_API virtual void set_color(const Color& new_color) {if (new_color == m_transformations.color) return; m_transformations.color = new_color; transformations_update();}
		NESSENGINE_API virtual void add_color(const Color& new_color) {set_color(get_color() + new_color);}
		NESSENGINE_API virtual void multiply_color(const Color& new_color) {set_color(get_color() * new_color);}
		NESSENGINE_API virtual void set_opacity(float opacity) {if (opacity == m_transformations.color.a) return; m_transformations.color.a = opacity; transformations_update();}
		NESSENGINE_API virtual void add_opacity(float opacity) {set_opacity(get_opacity() + opacity);}
		NESSENGINE_API virtual void multiply_opacity(float opacity) {set_opacity(get_opacity() * opacity);}
		NESSENGINE_API virtual void set_blend_mode(EBlendModes new_mode) {if (m_transformations.blend == new_mode) return; m_transformations.blend = new_mode; transformations_update();}
		NESSENGINE_API virtual void set_zindex(float new_z) {if (m_transformations.zorder == new_z) return; m_transformations.zorder = new_z; transformations_update();}
		NESSENGINE_API virtual void flip_x() {m_transformations.scale.x *= -1; transformations_update();}
		NESSENGINE_API virtual void flip_y() {m_transformations.scale.y *= -1; transformations_update();}
		NESSENGINE_API virtual const Point& get_position() const {return m_transformations.position;}
		NESSENGINE_API virtual const Size& get_scale() const {return m_transformations.scale;}
		NESSENGINE_API virtual const float get_rotation() const {return m_transformations.rotation;}
		NESSENGINE_API virtual const Color& get_color() const {return m_transformations.color;}
		NESSENGINE_API virtual const float get_opacity() const {return get_color().a;}
		NESSENGINE_API virtual const EBlendModes get_blend_mode() const {return m_transformations.blend;}
		NESSENGINE_API virtual float get_zindex() const {return m_transformations.zorder;}
		NESSENGINE_API virtual Point get_direction_vector() const {return Point::from_angle(get_rotation());}

		// return absolute position
		NESSENGINE_API virtual const Point& get_absolute_position() {return get_absolute_transformations().position;}
		NESSENGINE_API virtual const Point& get_absolute_position() const {return get_absolute_transformations_const().position;}

		// return absolute z index
		NESSENGINE_API virtual float get_absolute_zindex() {return get_absolute_transformations().zorder;}

		// called whenever transformations are updated
		NESSENGINE_API virtual void transformations_update() {}
		NESSENGINE_API virtual bool need_transformations_update() = 0;

		// return the absolute transformations of this object, with its parents taken into consideration
		NESSENGINE_API virtual const SRenderTransformations& get_absolute_transformations() = 0;
		NESSENGINE_API virtual const SRenderTransformations& get_absolute_transformations_const() const = 0;
	};

};