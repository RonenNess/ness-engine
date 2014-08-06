/**
* Canvas is a special type of sprite - instead of using an existing texture it creates an empty texture and allow other nodes and entities to render on it.
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "sprite.h"

namespace Ness
{

	/*
	* Canvas is a sprite that creates an empty texture and let other nodes and entities render on that texture.
	* with this object you can create post-render effects. for example, render your entire scene on a canvas and apply effects on that canvas like rotation or color tilt.
	*/
	class Canvas : public Sprite
	{
	private:
		bool				m_auto_clear;			// should we clear the canvas texture after every render automatically?
		unsigned int		m_last_clear_time;		// last time we cleaned the canvas texture
		Color				m_clean_color;			// color to clean canvas texture to
	public:

		// create the canvas.
		// TextureName is the name of the texture in the resource manager
		// size is the size of the canvas texture, if ZERO will use entire screen size
		NESSENGINE_API Canvas(Renderer* renderer, const String& NewTextureName, const Sizei& size = Sizei::ZERO);

		// if true, will clear this canvas after every frame rendered
		NESSENGINE_API inline void set_auto_clean(bool enabled) {m_auto_clear = enabled;}

		// clear the canvas
		NESSENGINE_API void clear();

		// set clean color
		NESSENGINE_API inline void set_clean_color(const Color& color) {m_clean_color = color;}
		NESSENGINE_API inline const Color& get_clean_color() {return m_clean_color;}

		// render the canvas
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera);
	};

	typedef SharedPtr<Canvas> CanvasPtr;

};
