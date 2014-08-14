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

	// the API of a renderable entity (entities are contained in a node)
	class EntityAPI : public RenderableAPI
	{
	public:
		// create the entity
		NESSENGINE_API EntityAPI(Renderer* renderer) : 
			RenderableAPI(renderer) {}

		// is it node or entity?
		NESSENGINE_API virtual bool is_node() const {return false;}
		NESSENGINE_API virtual bool is_entity() const {return true;}

	};

	// entity pointer
	NESSENGINE_API typedef SharedPtr<EntityAPI> EntityPtr;
};