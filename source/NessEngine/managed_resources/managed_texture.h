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
* A managed texture (texture loaded via the resources manager)
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <memory>
#include "../resources/TextureSheet.h"
#include "managed_resource.h"

namespace Ness
{
	namespace ManagedResources
	{
		// predeclare
		class ResourcesManager;

		// a texture inside the resources manager
		class ManagedTexture : public Resources::TextureSheet, public ManagedResource
		{
		public:
			// ctor for loading texture from file
			ManagedTexture(const String& file_name, SDL_Renderer* renderer, const Colorb* ColorKey = nullptr) : TextureSheet(file_name, renderer, ColorKey) {}

			// ctor for creating empty texture from size
			ManagedTexture(SDL_Renderer* renderer, const Sizei& size) : TextureSheet(renderer, size) {}
		};

		// a manager texture pointer
		NESSENGINE_API typedef SharedPtr<ManagedTexture> ManagedTexturePtr;
	};
};