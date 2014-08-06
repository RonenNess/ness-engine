/**
* A managed resource. everything in the resources manager must inherit from it (textures, fonts, etc..)
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../basic_types/containers.h"

namespace Ness
{
	namespace ManagedResources
	{

		// predeclare resources manager
		class ResourcesManager;

		// a texture inside the resources manager
		class ManagedResource
		{
		public:
			ResourcesManager*	rc_mng_manager;		// pointer to the resource manager containing this resource
			String				rc_mng_name;		// name of the resource inside the resources manager
		};
	};
};