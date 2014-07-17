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
		}
	};
};