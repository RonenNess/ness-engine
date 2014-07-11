/**
* A class that manage all the loaded resources
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <string>
#include <unordered_map>
#include "managed_texture.h"

namespace Ness
{
	namespace ManagedResources
	{

		// Texture as it stored in the resources manager
		struct TextureInManager
		{
			unsigned int	ref_count;
			ManagedTexture* texture;
		};

		/**
		* the resources manager
		*/
		class ResourcesManager
		{
		private:
			std::unordered_map<std::string, TextureInManager>	m_textures;			// map that holds all loaded textures
			std::string											m_base_path;		// basic path to search resources under
			Colorb												m_color_key;		// transparency color key
			bool												m_use_color_key;	// enable/disable color key
			SDL_Renderer*										m_renderer;			// our sdl renderer

		public:

			// create the resource manager and set defaults
			ResourcesManager();

			// set the renderer for this resources manager
			inline void set_renderer(SDL_Renderer* renderer) {m_renderer = renderer;}

			// get/load a texture
			ManagedTexturePtr get_texture(const std::string& textureName);

			// create an empty texture you can render on (use as rendering target). 
			// this texture will be added to the resource manager and you can later get it with get_texture()
			// if size is ZERO, will use entire screen size
			ManagedTexturePtr create_blank_texture(const std::string& textureName, const Sizei& size = Sizei::ZERO);

			// set the colorkey for this renderer
			// every texture loaded after this set will turn all pixels in the color key to transparent
			void set_color_key(const Colorb& color) {m_color_key = color; m_use_color_key = true;}
			void disable_color_key() {m_use_color_key = false;}

			// destroy the resources manager and clear all resources
			~ResourcesManager();

			// set basic path to search for resources under
			inline void set_resources_path(const std::string& path) {m_base_path = path;}

			// when a texture is removed (no longer referenced and deleted), it calls this function to be removed from the textures map as well
			// DONT USE THIS ON YOUR OWN, it supposed to happen automatically when texture has no more references.
			void delete_texture(const std::string& textureName);

		};
	};
};