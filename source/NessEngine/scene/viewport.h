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
* A viewport is a special region on which we can render scenes.
* you can render without a viewport. when you render a scene without viewport it will just render on the
* entire screen without any special settings. however, a viewport let you define the following settings:
* 1. viewport source size, i.e. the size in pixels of the region you can render on (this also affect culling)
* 2. viewport dest size, i.e. the size in pixels of the viewport as it will be rendered on screen. if your source size
*		is 100x100 and the dest size is 200x200, it means every pixel on this viewport will be doubled.
* 3. viewport dest position, i.e. where to render the viewport.
* 4. viewport source position, i.e. extra offset for all renderings.
* 5. add a black and white mask texture that acts as the alpha channels of the viewport. you can use it to create a
*		viewport in any shape and size.
* 6. basic transformations: color, scale, rotation, flip etc..
*
* example of cases where you need a viewport:
*	1. you want to render a scene inside a special shape based on a mask. for example a mini-map at the corner of the screen with a circular shape.
*	2. when you want to flip or rotate the entire screen, zoom in and out, do color tilt on everything, etc..
*	3. limit your render region or create split-screen effect. for example to create split-screen for two players: create two viewports; one for the
*		left side and one for the right side of the screen. each viewport will be rendered with a different camera based on the player position.
*	4. create post-render effects that are based on texture manipulation (for example add blurry effect etc)
*
* its important to understand that a viewport is meant to be something static, i.e. a region on screen to render on. don't try to use a viewport like a sprite.
* if you want to create effects like portals that show a different scene inside or windows you can look outside, don't use a viewport, use a Canvas instead. you can 
* a Canvas can do everything a Viewport does, but its API is more fitting for a renderable sprite entity.
*
* note1: the viewport is implemented with the Canvas entity. its very easy to implement your own viewport.
* note2: if you only use the viewport to zoom-in or zoom-out, please consider using renderer->set_renderer_size() instead.
*
* Author: Ronen Ness
* Since: 01/1015
*/

#pragma once
#include "../exports.h"
#include "../exceptions/exceptions.h"
#include "../renderable/entities/canvas.h"
#include "camera.h"

namespace Ness
{
	/**
	* The viewport entity you can render with. read header file top comment for more info.
	*/
	class Viewport
	{
	private:
		Renderer*		m_renderer;			// pointer to the parent renderer
		CanvasPtr		m_canvas;			// the canvas that will be used for this viewport
		Point			m_source_offset;	// viewport source offset, all rendering positions will start with this offset
		
	public:
		// create the viewport.
		// size is the source size of the viewport, i.e. the size of the region you can render on.
		//		if ZERO, will take the whole renderer size (note that if renderer / window size changes it
		//		will not update the size of the viewport automatically, you will need to update manually.
		NESSENGINE_API Viewport(Renderer* renderer, const Sizei& source_size = Sizei::ZERO);

		// reset the viewport with a new source size. note: this cancel all settings and transformations you've made.
		// size ZERO will take the entire renderer size (at the time of calling reset).
		NESSENGINE_API void reset(const Sizei& source_size = Sizei::ZERO);

		// set viewport destination size and position
		NESSENGINE_API void set_dest_position(const Point& position);
		NESSENGINE_API void set_dest_size(const Size& size);
		NESSENGINE_API void set_dest_region(const Rectangle& region);

		// set scale (multiplies dest region size).
		// negative scale flips
		NESSENGINE_API void set_scale(const Point& scale);

		// set color
		NESSENGINE_API void set_color(const Color& color);

		// set rotation
		NESSENGINE_API void set_rotation(float rotation);

		// flip on x and y axis. note: this negative the scale
		NESSENGINE_API void flip_x();
		NESSENGINE_API void flip_y();

		// set viewport source position, i.e. offset to add to all rendering on this viewport
		NESSENGINE_API inline void set_source_position(const Point& position) {m_source_offset = position;}

		// get the viewport source position
		NESSENGINE_API inline const Point& get_source_position() const {return m_source_offset;}
		NESSENGINE_API inline Point get_source_position() {return m_source_offset;}

		// set a black and white mask to this viewport that will act as alpha channels
		// black is opaque, white is invisible
		NESSENGINE_API void set_mask(const String& textureFile);

		// remove a mask from this canvas
		NESSENGINE_API void remove_mask();

		// set anchor for rotation and position
		NESSENGINE_API void set_anchor(const Point& anchor);

		// get the canvas texture of this viewport, i.e. the viewport "screen"
		NESSENGINE_API inline ManagedResources::ManagedTexturePtr get_canvas_texture() const {return m_canvas->get_texture();}

		// render the viewport
		NESSENGINE_API void render();
	};

	// scene pointer
	NESSENGINE_API typedef SharedPtr<Viewport> ViewportPtr;
};