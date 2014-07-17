/**
* A managed texture (texture loaded via the resources manager)
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <memory>
#include "../resources/font.h"
#include <unordered_map>
#include "managed_resource.h"

namespace Ness
{
	namespace ManagedResources
	{
		// predeclare
		class ResourcesManager;

		// a texture inside the resources manager
		class ManagedFont : public Resources::LoadedFont, public ManagedResource
		{
		public:
			ManagedFont(const String& fileName, int fontSize = 12) : LoadedFont(fileName, fontSize) {}
		};

		// a manager texture pointer
		NESSENGINE_API typedef NessSharedPtr<ManagedFont> ManagedFontPtr;
	};
};