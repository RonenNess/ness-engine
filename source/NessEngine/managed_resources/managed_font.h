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
			ManagedFont(const String& file_name, unsigned int font_size = 12) : LoadedFont(file_name, font_size) {}
		};

		// a manager font pointer
		NESSENGINE_API typedef SharedPtr<ManagedFont> ManagedFontPtr;
	};
};