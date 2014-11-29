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

#include "TextureSheet.h"
#include "../exceptions/exceptions.h"
#include <iostream>
#include <fstream>
#include "../renderer/renderer.h"

namespace Ness
{
	namespace Resources
	{
		TextureSheet::TextureSheet(const String& fileName, SDL_Renderer* renderer, const Colorb* ColorKey) : m_texture(nullptr)
		{
			load_file(fileName.c_str(), renderer, ColorKey);
		}

		TextureSheet::TextureSheet(SDL_Renderer* renderer, const Sizei& size) : m_texture(nullptr)
		{
			create_blank(renderer, size);
		}

		// destroy the texture
		TextureSheet::~TextureSheet()
		{
			if (m_texture)
			{
				SDL_DestroyTexture( m_texture );
			}
		}

		// create this texture as blank texture you can render on
		void TextureSheet::create_blank(SDL_Renderer* renderer, const Sizei& size)
		{
			m_size = size;
			m_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, size.x, size.y);
		}

		// load texture from file
		void TextureSheet::load_file(const char* fileName, SDL_Renderer* renderer, const Colorb* ColorKey)
		{

			// load the image
			SDL_Surface* loadedSurface = IMG_Load( fileName );
			if( loadedSurface == nullptr )
			{
				throw FailedToLoadTextureFile(fileName, IMG_GetError());
			}

			// set color key (if provided)
			if (ColorKey)
			{
				SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, ColorKey->r, ColorKey->g, ColorKey->b));
			}

			// store size
			m_size.x = loadedSurface->w;
			m_size.y = loadedSurface->h;

			// create the texture
			m_texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		
			// free the surface
			SDL_FreeSurface( loadedSurface );

			// failed to create texture??
			if( m_texture == nullptr )
			{
				throw FailedToLoadTextureFile(fileName, IMG_GetError());
			}

			// set filename
			m_file_name = fileName;
		}
	};
};