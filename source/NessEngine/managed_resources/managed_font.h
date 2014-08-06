/**
* A managed loadable font (fonts are loaded via the resources manager)
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <memory>
#include "../resources/font.h"
#include "managed_resource.h"

namespace Ness
{
	namespace ManagedResources
	{
		// predeclare the resources manager
		class ResourcesManager;

		// a font inside the resources manager
		class ManagedFont : public Resources::LoadedFont, public ManagedResource
		{
		public:
			ManagedFont(const String& fileName, int fontSize = 12) : LoadedFont(fileName, fontSize) {}
		};

		// a manager font pointer
		NESSENGINE_API typedef SharedPtr<ManagedFont> ManagedFontPtr;
	};
};