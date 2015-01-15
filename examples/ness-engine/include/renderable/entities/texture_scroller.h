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
* Texture Scrollers are objects that render tiled texture that can scroll based on camera
* for example, this is useful for animated backgrounds
* Author: Ronen Ness
* Since: 10/1014
*/

#pragma once
#include "sprite.h"

namespace Ness
{

	/*
	* a tiled texture that scroll based on camera position and factor (useable for repeating backgrounds)
	*/
	class TextureScroller : public Entity
	{
	protected:
		ManagedResources::ManagedTexturePtr		m_texture;
		Pointf									m_camera_speed_factor;
		Pointi									m_camera_offset;
		Pointi									m_base_offset;
		Point									m_uv;
		
	public:

		// create the texture scroller sprite with or without texture
		NESSENGINE_API TextureScroller(Renderer* renderer, ManagedResources::ManagedTexturePtr texture);
		NESSENGINE_API TextureScroller(Renderer* renderer, const String& TextureFile);

		// set defaults (based on Sprite::Defaults)
		NESSENGINE_API virtual void set_defaults();

		// change texture
		NESSENGINE_API inline void change_texture(ManagedResources::ManagedTexturePtr NewTexture) {m_texture = NewTexture;}
		NESSENGINE_API void change_texture(const String& NewTextureFile);

		// set how this texture will scroll based on camera position. 1.0f means it will scroll with the camera movement, 0.5f means
		// will scroll half the camera movement speed, 2.0f double camera speed, 0.0f means no affect by camera, ect..
		NESSENGINE_API void set_camera_move_factor(const Pointf& CamMoveFac) {m_camera_speed_factor = CamMoveFac;}
		NESSENGINE_API const Pointf& get_camera_move_factor() const {return m_camera_speed_factor;}

		// render this texture scroller
		NESSENGINE_API virtual void render(const CameraApiPtr& camera);

	protected:

		// init the sprite from a given texture
		void init_from_texture(ManagedResources::ManagedTexturePtr texture);

		// the actual rendering function
		NESSENGINE_API virtual void do_render(const Rectangle& target, const SRenderTransformations& transformations);
	};

	// sprite pointer type
	NESSENGINE_API typedef SharedPtr<TextureScroller> TextureScrollerPtr;
};