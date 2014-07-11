/**
* A managed resource. everything in the resources manager must inherit from it
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include <string>

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
			std::string			rc_mng_name;
		};
	};
};