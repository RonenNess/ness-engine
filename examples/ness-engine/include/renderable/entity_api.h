/**
* the API for a renderable entity
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "renderable_api.h"

namespace Ness
{
	class NodeAPI;

	// the API of a renderable entity
	class EntityAPI : public RenderableAPI
	{
	public:
		// create the entity
		NESSENGINE_API EntityAPI(Renderer* renderer) : 
			RenderableAPI(renderer) {}

	};

	// entity pointer
	NESSENGINE_API typedef SharedPtr<EntityAPI> EntityPtr;
};