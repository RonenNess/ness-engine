/**
* A managed texture (texture loaded via the resources manager)
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../texture/TextureSheet.h"
#include <unordered_map>
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
			ManagedTexture(const std::string& fileName, SDL_Renderer* renderer, const Colorb* ColorKey = nullptr) : TextureSheet(fileName, renderer, ColorKey) {}
			ManagedTexture(SDL_Renderer* renderer, const Sizei& size) : TextureSheet(renderer, size) {}
		};

		// a manager texture pointer
		typedef std::shared_ptr<ManagedTexture> ManagedTexturePtr;
	};
};