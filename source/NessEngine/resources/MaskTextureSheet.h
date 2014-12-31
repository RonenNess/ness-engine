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
* Define the Mask Texture Sheet - a special texture used for masking rendering (effect
* similar to stencil buffer). a mask texture must be black with white background.
* Author: Ronen Ness
* Since: 10/1014
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
		* represent a loadable texture used for masking (effect similar to stencil buffer)
		*/
		class MaskTextureSheet
		{
		private:
			SDL_Texture*	m_texture;			// the base mask texture (should be black with white background)
			SDL_Texture*	m_texture_invert;	// the mask texture inverted
			Sizei			m_size;				// size of the masked texture

		public:
			// create the texture sheet from file
			NESSENGINE_API MaskTextureSheet(const String& file_name, SDL_Renderer* renderer);

			// destroy the texture
			NESSENGINE_API ~MaskTextureSheet();

			// get texture size in pixels
			NESSENGINE_API inline const Sizei& get_size() const {return m_size;}

			// get the texture
			NESSENGINE_API inline SDL_Texture* texture() const {return m_texture;}

			// get the invert texture
			NESSENGINE_API inline SDL_Texture* invert_texture() const {return m_texture_invert;}

		private:
			// load and init this texture from file. this should be called only once!
			void load_file(const char* file_name, SDL_Renderer* renderer);

		};
	};
};