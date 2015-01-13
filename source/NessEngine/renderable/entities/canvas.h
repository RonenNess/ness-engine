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
* Canvas is a special type of sprite - instead of using an existing texture it creates an empty texture and allow other nodes and entities to render on it.
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "sprite.h"
#include "../../managed_resources/managed_mask_texture.h"

namespace Ness
{

	/*
	* Canvas is a sprite that creates an empty texture and let other nodes and entities render on that texture.
	* with this object you can create post-render effects. for example, render your entire scene on a canvas and apply effects on that canvas like rotation or color tilt.
	*/
	class Canvas : public Sprite
	{
	private:
		bool										m_auto_clear;			// should we clear the canvas texture after every render automatically?
		unsigned int								m_last_clear_time;		// last time we cleaned the canvas texture
		Color										m_clean_color;			// color to clean canvas texture to
		ManagedResources::ManagedMaskTexturePtr		m_mask;					// optional mask texture to apply on this canvas
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

		// set a mask texture on this canvas
		// the mask will act as the alpha channels of the canvas, where black is visible and white is invisibile.
		// this allows you to create a "shape" to the canvas.
		// NOTE! when mask is applied canvas does not support blending modes - the mask replace the blending operations.
		NESSENGINE_API void set_mask(const String& textureFile);

		// remove a mask from this canvas
		NESSENGINE_API inline void remove_mask() {m_mask.reset();}
	};

	typedef SharedPtr<Canvas> CanvasPtr;

};
