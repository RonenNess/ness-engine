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
* A static node is a node that renders everything on a matrix of canvases (called batches), and then renders them.
* this can greatly optimize things that are static, like tilemap or background stuff, by reducing the amounts of drawings
* Author: Ronen Ness
* Since: 07/1014
*/

#pragma once
#include "node.h"

namespace Ness
{
	/*
	* A static node that converts all entities and nodes under it into a matrix of textures, thus reducing rendering calls.
	* this is an optimizing node. after you fill it with entities and nodes call "build()" and then you can render it regulary.
	*/
	class StaticNode : public Node
	{
	private:
		Containers::UnorderedMap< int, Containers::UnorderedMap<int, CanvasPtr> >	m_batches;
		Sizei																		m_batch_size;
		unsigned int																m_unique_id;

	public:
		// create the static node. be sure to call 'build' after setting all the objects to generate the batches.
		// batchSize - the size of every single batch (canvas) in this static node.
		NESSENGINE_API StaticNode(Renderer* renderer, const Sizei& batchSize = Sizei(512, 512));

		// build all the static batches (the matrix of canvases)
		// if removeEntities = true, after the build all entities and nodes will be removed from this node. those
		// who have no more references will be deleted.
		NESSENGINE_API void build(bool removeEntities = true);

		// render the static batches
		NESSENGINE_API virtual void render(const CameraPtr& camera = NullCamera);

		// get a list with ALL the son entities that are currently visible in screen
		NESSENGINE_API virtual void __get_visible_entities(RenderablesList& out_list,
			const CameraPtr& camera = NullCamera, bool break_son_nodes = true);

	protected:
		// return the range of visible batches
		NESSENGINE_API Rectangle get_batches_range(const CameraPtr& camera);
	};

	// scene pointer
	NESSENGINE_API typedef SharedPtr<StaticNode> StaticNodePtr;
};