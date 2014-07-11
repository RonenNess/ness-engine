/**
* Define the Point class.
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../exports.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "../primitives/primitives.h"

namespace Ness
{
	namespace Resources
	{
		/**
		* represent a loadable texture sheet
		*/
		class TextureSheet
		{
		private:
			SDL_Texture*	m_texture;
			Sizei			m_size;

		public:
			// create the texture sheet from file
			// if ColorKey is provided, this color will turn invisible
			TextureSheet(const std::string& fileName, SDL_Renderer* renderer, const Colorb* ColorKey = nullptr);

			// create the texture sheet as blank texture you can render on
			TextureSheet(SDL_Renderer* renderer, const Sizei& size);

			// destroy the texture
			~TextureSheet();

			// get texture size in pixels
			inline const Sizei& get_size() const {return m_size;}

			// get the surface of this texture
			inline SDL_Texture* texture() const {return m_texture;}

		private:
			// load and init this texture from file. this should be called only once!
			void load_file(const char* fileName, SDL_Renderer* renderer, const Colorb* ColorKey = nullptr);

			// create this texture as empty texture you can render on
			void create_blank(SDL_Renderer* renderer, const Sizei& size);

		};
	};
};