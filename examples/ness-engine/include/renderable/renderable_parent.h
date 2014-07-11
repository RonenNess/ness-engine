/**
* the API for a class that wishes to be a parent for a renderable object
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once

namespace Ness
{
	class Renderable;

	// defined in renderable.h
	struct SRenderTransformations;

	// the API for a class that can be a parent to a renderable object (like a node or a scene)
	class RenderableParent
	{
	public:

		// get absolute transformations of this parent node
		virtual const SRenderTransformations& get_absolute_transformations() = 0;
	};

};