/**
* renderable basic shapes
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "renderable_entity.h"

namespace Ness
{

	// the renderable sprite class
	class RectangleShape: public RenderableEntity
	{
	protected:

	};

	// rectangle shape pointer
	NESSENGINE_API typedef std::shared_ptr<RectangleShape> RectangleShapePtr;
};