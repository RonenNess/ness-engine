/**
* A managed resource. everything in the resources manager must inherit from it
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "../basic_types/containers.h"

namespace Ness
{
	namespace ManagedResources
	{

		// predeclare
		class ResourcesManager;

		// a texture inside the resources manager
		class ManagedResource
		{
		public:
			ResourcesManager*	rc_mng_manager;
			String			rc_mng_name;
		};
	};
};