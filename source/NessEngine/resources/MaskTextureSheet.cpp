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

#include "MaskTextureSheet.h"
#include "../exceptions/exceptions.h"
#include <iostream>
#include <fstream>
#include "../renderer/renderer.h"

namespace Ness
{
	namespace Resources
	{
		MaskTextureSheet::MaskTextureSheet(const String& file_name, SDL_Renderer* renderer) : m_texture(nullptr)
		{
			load_file(file_name.c_str(), renderer);
		}

		// destroy the texture
		MaskTextureSheet::~MaskTextureSheet()
		{
			if (m_texture)
			{
				SDL_DestroyTexture( m_texture );
			}
			if (m_texture_invert)
			{
				SDL_DestroyTexture( m_texture_invert );
			}
		}

		// load texture from file
		void MaskTextureSheet::load_file(const char* file_name, SDL_Renderer* renderer)
		{

			// load the image
			SDL_Surface* loadedSurface = IMG_Load( file_name );
			if( loadedSurface == nullptr )
			{
				throw FailedToLoadTextureFile(file_name, IMG_GetError());
			}

			// store size
			m_size.x = loadedSurface->w;
			m_size.y = loadedSurface->h;

			// create the texture
			m_texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);

			// now invert the surface pixels and create the inverted texture
			for (int i = 0; i < loadedSurface->w * loadedSurface->h * loadedSurface->format->BytesPerPixel; i++)
			{
				// if has alpha channel and this is the alpha value skip it
				if ((loadedSurface->format->BytesPerPixel == 4) && ((i + 1) % 4 == 0))
				{
					continue;
				}

				// invert the channel
				Uint8* currPixel = ((Uint8*)loadedSurface->pixels) + i;
				*currPixel = ~(*currPixel);	
			}
			m_texture_invert = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		
			// free the surface
			SDL_FreeSurface( loadedSurface );

			// failed to create texture??
			if( m_texture == nullptr )
			{
				throw FailedToLoadTextureFile(file_name, IMG_GetError());
			}
		}
	};
};