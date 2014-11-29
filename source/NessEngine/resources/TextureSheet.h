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
* Define the Texture Sheet
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../exports.h"
#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "../basic_types/containers.h"
#include "../basic_types/all_basic_types.h"

namespace Ness
{
	namespace Resources
	{
		/**
		* represent a loadable texture
		*/
		class TextureSheet
		{
		private:
			SDL_Texture*	m_texture;
			Sizei			m_size;
			String			m_file_name;

		public:
			// create the texture sheet from file
			// if ColorKey is provided, this color will turn invisible
			NESSENGINE_API TextureSheet(const String& fileName, SDL_Renderer* renderer, const Colorb* ColorKey = nullptr);

			// create the texture sheet as blank texture you can render on
			NESSENGINE_API TextureSheet(SDL_Renderer* renderer, const Sizei& size);

			// return texture filename (if not loaded from file will be empty string)
			NESSENGINE_API inline const String& get_file_name() const {return m_file_name;}

			// destroy the texture
			NESSENGINE_API ~TextureSheet();

			// get texture size in pixels
			NESSENGINE_API inline const Sizei& get_size() const {return m_size;}

			// get the surface of this texture
			NESSENGINE_API inline SDL_Texture* texture() const {return m_texture;}

		private:
			// load and init this texture from file. this should be called only once!
			void load_file(const char* fileName, SDL_Renderer* renderer, const Colorb* ColorKey = nullptr);

			// create this texture as empty texture you can render on
			void create_blank(SDL_Renderer* renderer, const Sizei& size);

		};
	};
};