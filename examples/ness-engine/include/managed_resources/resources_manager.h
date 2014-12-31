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
* A class that manage all the loaded resources
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../exports.h"
#include "../basic_types/containers.h"
#include "managed_texture.h"
#include "managed_mask_texture.h"
#include "managed_font.h"

namespace Ness
{
	// predeclare renderer class
	class Renderer;

	namespace ManagedResources
	{

		// Texture as it stored in the resources manager with reference count
		struct __STextureInManager
		{
			unsigned int	ref_count;
			ManagedTexture* texture;
		};

		// Mask Texture as it stored in the resources manager with reference count
		struct __SMaskTextureInManager
		{
			unsigned int		ref_count;
			ManagedMaskTexture* texture;
		};

		// Font as it stored in the resources manager with reference count
		struct __SFontInManager
		{
			unsigned int	ref_count;
			ManagedFont*	font;
		};

		/**
		* the resources manager - manage all the resources loaded to memory (textures, fonts, etc..) and responsible
		* to unload them automatically when no longer used.
		*/
		class ResourcesManager
		{
		private:
			Containers::UnorderedMap<String, __STextureInManager>		m_textures;			// map that holds all loaded textures
			Containers::UnorderedMap<String, __SMaskTextureInManager>	m_mask_textures;	// map that holds all loaded mask textures
			Containers::UnorderedMap<String, __SFontInManager>			m_fonts;			// map that holds all loaded fonts
			String														m_base_path;		// basic path to search resources under
			Colorb														m_color_key;		// transparency color key
			bool														m_use_color_key;	// enable/disable color key
			Renderer*													m_renderer;			// pointer to the renderer manager
			bool														m_destroyed;		// was it destroyed?

		public:

			// create the resource manager and set defaults
			NESSENGINE_API ResourcesManager();

			// set the renderer for this resources manager
			NESSENGINE_API inline void set_renderer(Renderer* renderer) {m_renderer = renderer;}

			// get/load a texture
			NESSENGINE_API ManagedTexturePtr get_texture(const String& textureName);

			// get/load a masked texture
			NESSENGINE_API ManagedMaskTexturePtr get_mask_texture(const String& textureName);

			// get/load a font
			NESSENGINE_API ManagedFontPtr get_font(const String& fontName, unsigned int font_size = 12);

			// destroy the resources manager and anything in it
			// once called, this instance is no longer useable!
			NESSENGINE_API void destroy();

			// create an empty texture you can render on (use as rendering target). 
			// this texture will be added to the resource manager and you can later get it with get_texture()
			// if size is ZERO, will use entire screen size
			NESSENGINE_API ManagedTexturePtr create_blank_texture(const String& textureName, const Sizei& size = Sizei::ZERO);

			// set the colorkey for this renderer
			// every texture loaded after this set will turn all pixels in the color key to transparent
			NESSENGINE_API void set_color_key(const Colorb& color) {m_color_key = color; m_use_color_key = true;}
			NESSENGINE_API void disable_color_key() {m_use_color_key = false;}

			// destroy the resources manager and clear all resources
			NESSENGINE_API ~ResourcesManager();

			// set basic path to search for resources under
			NESSENGINE_API inline void set_resources_path(const String& path) {m_base_path = path;}

			// when a mask texture is removed (no longer referenced and deleted), it calls this function to be removed from the textures map as well
			// DONT USE THIS ON YOUR OWN, it supposed to happen automatically when texture has no more references.
			void __delete_mask_texture(const String& textureName);

			// when a texture is removed (no longer referenced and deleted), it calls this function to be removed from the textures map as well
			// DONT USE THIS ON YOUR OWN, it supposed to happen automatically when texture has no more references.
			void __delete_texture(const String& textureName);

			// when a font is removed (no longer referenced and deleted), it calls this function to be removed from the fonts map as well
			// DONT USE THIS ON YOUR OWN, it supposed to happen automatically when a font has no more references.
			void __delete_font(const String& fontName);

		};
	};
};