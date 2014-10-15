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
* A scene node that renders lighting. 
* basically will render 'ambient_color' all over the screen, and every object rendered will be added as additive rendering and make it brighter.
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "basic_node.h"
#include "../entities/canvas.h"

namespace Ness
{
	class Light : public Sprite
	{
	private:
		bool m_need_redraw;

	public:
		// create the light object
		NESSENGINE_API Light(Renderer* renderer, const String& TextureFile, const Color& color) 
			: Sprite(renderer, TextureFile) 
		{
			set_color(color);
			set_anchor(Point::HALF);
			m_need_redraw = false;
		}

		// set need transformations update + redraw
		NESSENGINE_API virtual void transformations_update()
		{
			m_need_transformations_update = m_need_redraw = true;
		}

		// return if this light needs redraw
		NESSENGINE_API inline bool need_redraw() const {return m_need_redraw;}

		// set no longer need redraw
		NESSENGINE_API inline void set_need_redraw(bool need) {m_need_redraw = need;}

		// need to update transformations when changing visible so that the light node will re-render
		NESSENGINE_API inline void set_visible(bool Visible) 
		{
			transformations_update();
			m_visible = Visible;
		}
	};

	NESSENGINE_API typedef SharedPtr<Light> LightPtr;

	// a special node that creates lighting effects.
	// how this works:
	// this node creates a canvas, and fill it with 'ambient light color' (default to black). 
	// every 'light' you add to this node is a sprite rendered with additive effect over the canvas.
	// when this node is rendered, the canvas is rendered all over the screen with modulus effect (so lights make it less dark).
	// note: the light node has optimization that the canvas is re-rendered only when one of the lights or the lightnode itself changes.
	// note2: the light node acts like a regular renderable node, with rendering order and even z-value. so make sure to add it last to affect all objects that should be below it.
	class LightNode : public BaseNode
	{
	private:
		CanvasPtr m_canvas;			// the light canvas
		bool m_need_update;			// true when need to re-render the light canvas
		bool m_always_update;		// if true, will always re-render the entire light canvas
		Point m_last_camera_pos;	// last camera position

	public:
		// create the znode
		NESSENGINE_API LightNode(Renderer* renderer);

		// set / get ambient color (ambient == the lighting where there is no light (default is black)
		NESSENGINE_API inline void set_ambient_color(const Color& color) {m_canvas->set_clean_color(color); m_need_update = true;}
		NESSENGINE_API inline const Color& get_ambient_color() const {return m_canvas->get_clean_color();}

		// when the lightnode updates, we need to re-render
		NESSENGINE_API virtual void transformations_update();

		// create a light
		NESSENGINE_API LightPtr create_light(const String& lightTexture, const Color& color = Color::WHITE);

		// return all lights currently in screen
		NESSENGINE_API void get_lights_in_screen(Containers::Vector<LightPtr>& out_list, const CameraPtr& camera = NullCamera) const;

		// render the light node
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera);

		// by default, the light node only refreash when one of the lights changes.
		// if you want from any reason to disable this optimization and always recalculate the lights, 
		// set this to true. this is not recommended, for the optimization should not affect you in any way and is very useful.
		NESSENGINE_API inline void set_always_refresh(bool enabled) {m_always_update = enabled;}

		// add/remove object
		NESSENGINE_API void add(const RenderablePtr& object);
		NESSENGINE_API void remove(const RenderablePtr& object);

		// return the light node canvas
		NESSENGINE_API const CanvasPtr& get_target_canvas() const {return m_canvas;}
	};

	// scene pointer
	NESSENGINE_API typedef SharedPtr<LightNode> LightNodePtr;
};